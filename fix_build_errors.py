import logging
from pathlib import Path
import subprocess
import time
import re

llm_path = "/Library/Frameworks/Python.framework/Versions/3.13/bin/llm"
model_name = "github/gpt-4o"
yaml_file = "yaml/fix_build_errors.yaml"
build_dir = Path("build")
tests_dir = Path("tests")
max_iterations = 5  # To avoid infinite loops

header_files = [
    "orgChartApi/models/Department.h",
    "orgChartApi/models/Person.h",
    "orgChartApi/models/Job.h",
    "orgChartApi/models/User.h",
    "orgChartApi/models/PersonInfo.h",
    "orgChartApi/controllers/DepartmentsController.h",
    "orgChartApi/controllers/AuthController.h",
    "orgChartApi/controllers/JobsController.h",
    "orgChartApi/controllers/PersonsController.h",
    "orgChartApi/utils/utils.h",
    "orgChartApi/plugins/Jwt.h",
    "orgChartApi/plugins/JwtPlugin.h",
    "orgChartApi/filters/LoginFilter.h"
]

logging.basicConfig(level=logging.INFO, format='%(levelname)s: %(message)s')

def read_file(filepath):
    try:
        with open(filepath, 'r', encoding='utf-8') as f:
            return f.read()
    except Exception as e:
        logging.error(f"Failed to read {filepath}: {e}")
        return ""

def strip_markdown_code_fences(output):
    if isinstance(output, list):
        output = "\n".join(output)
    lines = output.strip().splitlines()
    if lines and lines and lines[0].strip().startswith("```"):
        lines = lines[1:]
    if lines and lines[-1].strip().startswith("```"):
        lines = lines[:-1]
    return "\n".join(lines).strip()

def extract_cpp_code(output):
    idx = output.find("#include")
    if idx != -1:
        output = output[idx:]
    last_brace = output.rfind("}")
    if last_brace != -1:
        output = output[:last_brace+1]
    return output.strip()

def extract_relevant_errors(build_log, test_file):
    relevant_lines = []
    for line in build_log.splitlines():
        if test_file in line:
            relevant_lines.append(line)
    return "\n".join(relevant_lines)

def build_class_definitions(test_file_content, build_log):
    used_headers = []
    for header in header_files:
        if Path(header).exists():
            header_content = read_file(header)
            class_name = Path(header).stem
            # Only include headers if their class name is in the test file or build log
            if class_name in test_file_content or class_name in build_log:
                used_headers.append(header_content)
    return "\n\n".join(used_headers)

def call_llm(prompt, retries=3):
    for attempt in range(retries):
        try:
            logging.info("Calling LLM subprocess...")
            result = subprocess.run(
                [llm_path, "-m", model_name, "submit"],
                input=prompt,
                text=True,
                stdout=subprocess.PIPE,
                stderr=subprocess.PIPE,
                timeout=1800
            )
            logging.info("LLM subprocess finished.")
            if result.returncode != 0:
                logging.error(f"LLM error: {result.stderr.strip()}")
                time.sleep(2)
                continue
            output = result.stdout.strip()
            output = strip_markdown_code_fences(output)
            if not output:
                logging.error("LLM returned empty output after cleaning.")
                continue
            return output
        except subprocess.TimeoutExpired:
            logging.error("LLM subprocess timed out.")
            time.sleep(2)
        except Exception as e:
            logging.error(f"LLM execution failed: {e}")
            time.sleep(2)
    return None

def check_for_incomplete_tests(test_file):
    with open(test_file, "r", encoding="utf-8") as f:
        content = f.read()
    if "TODO: TEST INCOMPLETE" in content:
        logging.error(f"Test file {test_file} is incomplete! Build should not pass.")
        return False
    return True

def fix_test_file(test_file, yaml_file, build_log):
    code = read_file(test_file)
    instructions = read_file(yaml_file)
    if not code or not instructions:
        logging.error("Missing code or instructions for build fix.")
        return False
    relevant_errors = extract_relevant_errors(build_log, test_file)
    if not relevant_errors:
        logging.info(f"No relevant errors found for {test_file}, skipping.")
        return True
    class_definitions = build_class_definitions(code, relevant_errors)
    prompt = f"""
--- CLASS DEFINITIONS ---
{class_definitions}

--- YAML INSTRUCTIONS ---
{instructions}

--- BUILD LOG (Relevant Errors Only) ---
{relevant_errors}

--- C++ TEST FILE ---
{code}
"""
    # Optional: Print prompt size for debugging token limits
    logging.info(f"Prompt size: {len(prompt.split())} words, {len(prompt)} characters")
    output = call_llm(prompt)
    if not output:
        logging.error("No output from model.")
        return False
    output = extract_cpp_code(output)
    try:
        with open(test_file, "w", encoding="utf-8") as f:
            f.write(output)
        logging.info(f"Fixed build issues in: {test_file}")
        if not check_for_incomplete_tests(test_file):
            logging.error("Aborting: Incomplete test detected.")
            exit(1)
        return True
    except Exception as e:
        logging.error(f"Error writing test file: {e}")
        return False

def extract_error_files(build_log, test_dir="dummyTest"):
    pattern = re.compile(r'(' + re.escape(test_dir) + r'/[\w./\\-]+_test\.cc)[:(]')
    files = set()
    for line in build_log.splitlines():
        match = pattern.search(line)
        if match:
            file_path = match.group(1)
            files.add(file_path)
    return list(files)

def build_and_log():
    build_log_path = build_dir / "build.log"
    build_dir.mkdir(exist_ok=True)
    with open(build_log_path, "w") as logf:
        result = subprocess.run(
            ["make", "-C", str(build_dir)],
            stdout=logf,
            stderr=subprocess.STDOUT,
            text=True
        )
    return result.returncode, build_log_path

if __name__ == "__main__":
    logging.info("Starting automated build error fixing workflow.")
    iteration = 0
    while iteration < max_iterations:
        iteration += 1
        logging.info(f"--- Iteration {iteration} ---")
        returncode, build_log_path = build_and_log()
        build_log = read_file(build_log_path)
        if returncode == 0:
            logging.info("Build succeeded! All test files are fixed.")
            break
        error_files = extract_error_files(build_log, test_dir="dummyTest")
        if not error_files:
            logging.info("No test files with build errors found.")
            if "undefined symbol" in build_log.lower() or "undefined symbols" in build_log.lower() or "ld: symbol(s) not found" in build_log.lower():
                logging.error("Build failed due to linker error (e.g., missing main).")
                # Optionally, print the relevant lines:
                for line in build_log.splitlines():
                    if "undefined symbol" in line.lower() or "ld:" in line.lower():
                        logging.error(line)
            break
        logging.info(f"Files with build errors: {error_files}")
        for test_file in error_files:
            if not Path(test_file).exists():
                logging.warning(f"File {test_file} does not exist, skipping.")
                continue
            fix_test_file(test_file, yaml_file, build_log)
    else:
        logging.warning("Max iterations reached. Some build errors may remain.")
    logging.info("Build fixing workflow complete.")
