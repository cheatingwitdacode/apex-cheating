@ECHO OFF
copy "%~1" "apexbot\r5apex.bin"
cd apexbot
cargo run --release -- "r5apex.bin" > stdout.md
explorer .
pause