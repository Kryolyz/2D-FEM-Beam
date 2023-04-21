## Warning
This is one of the first things I've ever programmed and it shows. If you are an experienced programer I strongly advise against looking at the source code as it may or may not make you angry.

# 2D-FEM
A program to visualize and calculate forces, stresses and bending of a beam. Written in C.

I wrote this in my 2nd semester of college so don't expect anything efficient or aesthetic, however it is fully functional. Was great for learning C aswell.
Also I used the worst possible graphics library that exists in the entire internet. It's so old that I had to downgrade my compiler for it to function. I had to hardcode almost every piece of graphic on the screen, the only available functions let me dot individual pixel, draw lines, draw circles, write and change the color. So every arrow and box had to be put together with lines, which was quite exhausting, however it was also very straightforward to use.

A full documentation I wrote for an essay is available in the root folder.

Keep in mind that even though the documentation is written in english, the program (including variable names for buttons) is german. Using the docs that shouldn't be too big of a problem though.

# References
I basically just used the knowledge from my first and second semester of mechanical engineering so there's little written material I could reference here. It's basically force equations on the beam to get reaction forces of the bearings and using the geometry to get the resulting stress and bending. Iterate those equations with a certain step size over the entire beam, save the results and show them in a grpah. There are tons of programs out there that do the exact same. In fact you can type "calculate bending of a beam" into any search engine and the first 3 results aren't formulas but instead online calculators and visualizers.

# Disclaimer
The term "2D-FEM" is basically clickbait since the program doesn't actually use Finite Elements. However it does iterate over the entire beam to get individual values and plot them, just the method of getting these values is different.

