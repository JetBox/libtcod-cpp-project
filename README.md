# libtcod-cpp-project

## Local Setup

This repository uses [pre-commit](https://pre-commit.com/) hooks (including `clang-format`)
to keep the codebase consistent. After cloning, enable the hooks locally:

1. [Install Python](https://www.python.org/downloads/) if you don't already have it.
2. Install pre-commit:

   ```sh
   pip install pre-commit
   ```

3. Install the git hooks from the project directory:

   ```sh
   pre-commit install --hook-type pre-commit --hook-type pre-push
   ```

Once installed, the hooks run automatically on `git commit` and `git push`. If a hook
reformats or modifies files, the commit is aborted; re-stage the changed files and commit
again. To check every file in the repo at once, run:

```sh
pre-commit run --all-files
```
