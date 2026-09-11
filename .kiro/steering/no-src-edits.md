---
inclusion: always
---

# Source File Editing Policy

Do NOT edit, create, or delete any files under the `src/` directory. This includes
all source and header files (e.g. `.cpp`, `.h`, `.hpp`) within `src/`.

Instead, when a change to a `src/` file is needed:

- Explain the problem and the root cause.
- Provide the recommended fix as a code snippet in chat, clearly indicating the
  file and the location where it should go.
- Let the user apply the change themselves.

You MAY still read files under `src/` to diagnose issues and give accurate advice.

You MAY edit files outside of `src/` normally (e.g. build scripts, CMake config,
steering files), unless the user says otherwise.

If the user explicitly asks you to edit a specific `src/` file in a given message,
you may do so for that one request only; the default remains no `src/` edits.
