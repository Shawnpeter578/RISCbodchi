# Initialize git repository
git init

# Add your .gitignore file first
git add .gitignore
git commit -m "chore: add gitignore"

# Stage all project source files, headers, tests, and scripts
git add Makefile README.md runit.sh gui.py include/ src/ test/
git commit -m "feat: complete C backend core, unit tests, and Python GUI for Team Riscbodchi"

# Connect to your GitHub remote repository and push
git branch -M main
git remote add origin <YOUR_GITHUB_REPOSITORY_URL>
git push -u origin main