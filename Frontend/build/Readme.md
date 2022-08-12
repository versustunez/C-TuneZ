# Usage

*NOTES*
THIS NEED TO BE INSTALLED IN A SUBFOLDER!

Copy buildConfig.example.json to parent directory and rename it to buildConfig.json

Compiler Flags in the JS Tree are from "Google Closure Compiler"

# Install

```
git clone https://git.vstz.dev/versustunez/gulp-dynamic.git build
cd build
npm install
cp buildConfig.example.json ../buildConfig.json
```

# Config

### Variables

| Key   | VALUE               |
|-------|---------------------|
| $dir  | ROOT DIR            |
| $src  | SRC FROM Config     |
| $out  | OUT FROM CONFIG     |
| $name | NAME KEY FROM BLOCK |
| $     | BLOCK INPUT VALUE   |
|-------|---------------------|

### ROOT

| Key  | isRequired | TYPE   |
|------|------------|--------|
| src  | YES        | STRING |
| out  | YES        | STRING |
| js   | NO         | ARRAY  |
| scss | NO         | ARRAY  |

### JS-Array

| Key        | isRequired | TYPE   | DEFAULT               |
|------------|------------|--------|-----------------------|
| name       | YES        | STRING | -                     |
| minify     | NO         | BOOL   | false                 |
| onlyMinify | NO         | BOOL   | false                 |
| input      | YES        | STRING | -                     |
| output     | YES        | STRING | -                     |
| files      | YES        | ARRAY  | -                     |
| compiler   | NO         | OBJECT | #JS-Compiler Settings |

### JS-Compiler Settings

| Key               | isRequired | TYPE   | DEFAULT               |
|-------------------|------------|--------|-----------------------|
| compilation_level | NO         | STRING | SIMPLE                |
| warning_level     | NO         | STRING | VERBOSE               |
| language_in       | NO         | STRING | ECMASCRIPT6_STRICT    |
| language_out      | NO         | STRING | ECMASCRIPT6_STRICT    |
| js_output_file    | NO         | STRING | $name.min.js          |

*NOTES* SEE: [Google Compiler Flags](https://github.com/google/closure-compiler/wiki/Flags-and-Options)

## Example Config

```json
{
  "src": "$dir/src",
  "out": "$dir/public/out",
  "js": [
    {
      "name": "main",
      "minify": true,
      "onlyMinify": true,
      "input": "$src/js/",
      "output": "$out/js/",
      "files": [
        "app.js"
      ],
      "compiler": {
        "compilation_level": "ADVANCED",
        "warning_level": "VERBOSE",
        "language_in": "ECMASCRIPT6_STRICT",
        "language_out": "ECMASCRIPT6_STRICT",
        "js_output_file": "$name.min.js"
      }
    }
  ],
  "scss": [
    {
      "name": "main",
      "input": "$src/theme/**/*.scss",
      "output": "$out/theme/$name"
    }
  ]
}
```
