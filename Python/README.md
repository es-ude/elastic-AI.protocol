# EAIP: Python

## Include via astral-uv

To include this project with the uv package manger, include the following parts to your `pyproject.toml`:

```toml
[project]
dependencies = [
    ...
    "elasticai_protocol",
]

[tool.uv.sources]
elasticai_protocol = { git = "https://github.com/es-ude/elastic-AI.protocol.git", tag = "v.3.1.0" }
```
