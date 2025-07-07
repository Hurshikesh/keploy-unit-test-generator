import subprocess
import logging
from pathlib import Path
import os
import time

# -------- CONFIGURATION --------
llm_path = "/Library/Frameworks/Python.framework/Versions/3.13/bin/llm"
model_name = "github/gpt-4o"
files_to_test = [
        "orgChartApi/models/Department.cc"
        "orgChartApi/models/Person.h"
        "orgChartApi/models/Job.cc"
        "orgChartApi/models/User.cc"
        "orgChartApi/models/PersonInfo.cc"
        "orgChartApi/controllers/DepartmentsController.cc"
        "orgChartApi/controllers/AuthController.cc"
        "orgChartApi/controllers/JobsController.cc"
        "orgChartApi/controllers/PersonsController.cc"
        "orgChartApi/utils/utils.cc"
        "orgChartApi/plugins/Jwt.cc"
        "orgChartApi/plugins/JwtPlugin.cc"
        "orgChartApi/filters/LoginFilter.cc"
]
yaml_file = "yaml/testgen.yaml"
tests_dir = Path("tests")
logging.basicConfig(level=logging.INFO, format='%(levelname)s: %(message)s')

# -------- UTILITIES --------

def read_file(filepath):
    """Read a file and return its contents or None if error."""
    try:
        with open(filepath, 'r', encoding='utf-8') as f:
            return f.read()
    except Exception as e:
        logging.error(f"Failed to read {filepath}: {e}")
        return None

def strip_markdown_code_fences(output):
    """Remove markdown code fences from LLM output, if present."""
    lines = output.strip().splitlines()
    # Remove first line if it's a code fence
    if lines and lines[0].strip().startswith("```"):
        lines = lines[1:]
    # Remove last line if it's a code fence
    if lines and lines and lines[-1].strip().startswith("```"):
        lines = lines[:-1]
    return "\n".join(lines).strip()

def call_github_model(prompt: str, retries=3):
    """Call the LLM via subprocess and return the output."""
    for attempt in range(retries):
        try:
            logging.info("Calling LLM subprocess...")
            result = subprocess.run(
                [llm_path, "-m", model_name, "submit"],
                input=prompt,
                text=True,
                stdout=subprocess.PIPE,
                stderr=subprocess.PIPE,
                timeout=600
            )
            logging.info("LLM subprocess finished.")
            if result.returncode != 0:
                logging.error(f"GitHub Model error: {result.stderr.strip()}")
                time.sleep(2)
                continue
            output = result.stdout.strip()
            # Remove markdown code fences if present
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

def generate_test_for_file(source_file, yaml_file):
    """Generate a test file for a given C++ source file using the LLM."""
    code = read_file(source_file)
    instructions = read_file(yaml_file)
    if not code or not instructions:
        logging.error("Missing code or instructions.")
        return False

    prompt = f"""
You are an AI expert in Drogon C++ and unit testing with Google Test.

--- YAML INSTRUCTIONS ---
{instructions}

--- C++ SOURCE FILE ---
{code}

IMPORTANT:
- Include correct Drogon/Model headers.
- Do NOT use undefined symbols like DbClient unless declared.
- Avoid mocking unless absolutely needed.
- Use namespaces if code uses drogon_model::org_chart::Department, etc.
- Only write #include lines that compile.
- Return only compilable C++ test code in a single file using TEST() macros.
"""

    output = call_github_model(prompt)
    if not output:
        logging.error("No output from model.")
        return False

    out_filename = Path(source_file).stem + "_test.cc"
    out_path = tests_dir / out_filename
    try:
        tests_dir.mkdir(parents=True, exist_ok=True)
        with open(out_path, "w", encoding="utf-8") as f:
            f.write(output)
        logging.info(f"Test generated: {out_path}")
        return True
    except Exception as e:
        logging.error(f"Error writing test file: {e}")
        return False

# -------- MAIN WORKFLOW --------

if __name__ == "__main__":
    logging.info("Starting test generation workflow.")
    for file in files_to_test:
        success = generate_test_for_file(file, yaml_file)
        if not success:
            logging.error(f"Failed to generate test for {file}")
        else:
            logging.info(f"Successfully generated test for {file}")
    logging.info("All done.")
