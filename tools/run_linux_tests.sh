#!/bin/bash
rm build/Release-LinuxTests/Vic2ToHoi4ConverterTests
cmake -H. -Bbuild/linux-intermediate && cmake --build ./build/linux-intermediate -- -j40
(
cd build/Release-LinuxTests
./Vic2ToHoi4ConverterTests
)