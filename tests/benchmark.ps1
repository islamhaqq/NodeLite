$env:PATH += ";C:\Source\NodeLite\cmake-build-debug-visual-studio"
(Measure-Command { node tests\displayMessage.js }).TotalMilliseconds
(Measure-Command { nodelite tests\displayMessage.js }).TotalMilliseconds