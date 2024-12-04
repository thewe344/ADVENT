# Day four
## Nice day to look stuff up

With this one i was thinking of different ways to itterate through the charachters to find the different combinations. 

The simplest and most straight forward way i could come up with was using a look up table.

I had it all planned in my head, and it was the best solution ever, felt a bit satisfied that i figured that out. 

But to do this i had to sketch up the look upp tables ´´lookuptableDescription.txt´´ and ´´lookuptableDescriptionPart2.txt´´ so it would be easier to visualize them. To do this i needed to use an array (´´vector<vector>´´)´. The inner vector contains ´´pairs´´, that would hold positive or negative values that could be added to the index of my input string. 

For the input string, each line is divided in a vector aswell, so the look up table would make it possible to look up corresponding indexes in this array and follow a certain pattern.

It was a pleasent suprised when i got to part 2, where the only thing i needed to change was the lookup table, to look for the X-MAS, searching for every A. And i knew when one X-MAS had been found for one index, i could continue to the next because there could only be one X-MAS for each A.

Also added some boundry control so it wouldnt check A's at index 0 or the last, (because no X-MAS could be formed in the beginning or end).

More optimization could be done though, like for instance, when i had found one part of the X-MAS, but one part is wrong, i could add not to continue with lookup table x,y,z because they could not exist. But didnt feel to do that for now.

Overall, a neit solution!

