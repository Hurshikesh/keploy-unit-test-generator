import logging
from pathlib import Path
import subprocess
import time

llm_path = "/Library/Frameworks/Python.framework/Versions/3.13/bin/llm"
model_name = "github/gpt-4o"
tests_dir = Path("tests")
yaml_file = "yaml/improve_coverage.yaml"
coverage_report_file = "build/coverage.txt"
logging.basicConfig(level=logging.INFO, format='%(levelname)s: %(message)s')

def read_file(filepath):
    try:
        with open(filepath, 'r', encoding='utf-8') as f:
            return f.read()
    except Exception as e:
        logging.error(f"Failed to read {filepath}: {e}")
        return None

def strip_markdown_code_fences(output):
    lines = output.strip().splitlines()
    if lines and lines[0].strip().startswith("```"):
        lines = lines[1:]
    if lines and lines[-1].strip().startswith("```"):
        lines = lines[:-1]
    return "\n".join(lines).strip()

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
                timeout=600
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

def improve_test_file(test_file, yaml_file, coverage_report):
    code = read_file(test_file)
    instructions = read_file(yaml_file)
    if not code or not instructions:
        logging.error("Missing code or instructions for coverage improvement.")
        return False
    prompt = f"""
--- YAML INSTRUCTIONS ---
{instructions}

--- COVERAGE REPORT ---
{coverage_report}

--- C++ TEST FILE ---
{code}
"""
    output = call_llm(prompt)
    if not output:
        logging.error("No output from model.")
        return False
    try:
        with open(test_file, "w", encoding="utf-8") as f:
            f.write(output)
        logging.info(f"Coverage improved for: {test_file}")
        return True
    except Exception as e:
        logging.error(f"Error writing test file: {e}")
        return False

if __name__ == "__main__":
    logging.info("Starting coverage improvement.")
    coverage_report = read_file(coverage_report_file)
    if not coverage_report:
        logging.error("No coverage report found!")
        exit(1)
    for test_file in tests_dir.glob("*_test.cc"):
        improve_test_file(test_file, yaml_file, coverage_report)
    logging.info("Coverage improvement complete.")
