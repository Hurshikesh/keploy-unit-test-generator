import logging
from pathlib import Path
import subprocess
import time

llm_path = "/Library/Frameworks/Python.framework/Versions/3.13/bin/llm"
model_name = "github/gpt-4o"
tests_dir = Path("tests")
yaml_file = "yaml/refine_tests.yaml"
MAX_PROMPT_CHARS = 12000  # ~8000 tokens

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
    if lines and lines and lines[-1].strip().startswith("```"):
        lines = lines[:-1]
    return "\n".join(lines).strip()

def remove_llm_summary(output):
    """
    Removes LLM summary or notes from the output, keeping only the C++ code.
    Looks for common summary markers.
    """
    summary_markers = [
        "--- summary of changes ---",
        "--- summary ---",
        "## summary",
        "// summary:",
        "# summary",
        "/* summary",
        "Summary of changes:",
        "Summary:"
    ]
    lower_output = output.lower()
    for marker in summary_markers:
        idx = lower_output.find(marker)
        if idx != -1:
            return output[:idx].rstrip()
    return output

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

def refine_test_file(test_file, yaml_file):
    code = read_file(test_file)
    instructions = read_file(yaml_file)
    if not code or not instructions:
        logging.error("Missing code or instructions for refinement.")
        return False
    prompt = f"""
--- YAML INSTRUCTIONS ---
{instructions}

--- C++ TEST FILE ---
{code}
"""
    prompt_size = len(prompt)
    logging.info(f"Prompt size: {prompt_size} characters for {test_file}")
    if prompt_size > MAX_PROMPT_CHARS:
        logging.error(f"Prompt too large ({prompt_size} chars). Skipping {test_file}.")
        return False

    output = call_llm(prompt)
    if not output:
        logging.error("No output from model.")
        return False
    # Remove LLM summary if present
    output = remove_llm_summary(output)
    try:
        with open(test_file, "w", encoding="utf-8") as f:
            f.write(output)
        logging.info(f"Refined test: {test_file}")
        return True
    except Exception as e:
        logging.error(f"Error writing test file: {e}")
        return False

if __name__ == "__main__":
    logging.info("Starting test refinement.")
    for test_file in tests_dir.glob("*_test.cc"):
        refine_test_file(test_file, yaml_file)
    logging.info("Refinement complete.")
