def main():
    import os
    import subprocess

    repo_dir = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
    doxyfile_path = os.path.join(repo_dir, 'Doxyfile')
    output_dir = os.path.join(repo_dir, 'site', 'api')

    # Ensure output directory exists
    os.makedirs(output_dir, exist_ok=True)

    # Prepare environment to override OUTPUT_DIRECTORY
    env = os.environ.copy()
    env['OUTPUT_DIRECTORY'] = output_dir

    # Run doxygen with the Doxyfile
    try:
        subprocess.run(['doxygen', doxyfile_path], check=True, env=env)
        print(f"Doxygen documentation generated in {output_dir}")
    except subprocess.CalledProcessError as e:
        print(f"Error running doxygen: {e}")
    
if __name__ == "__main__":
    main()
