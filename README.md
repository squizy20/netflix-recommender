\# Netflix Movie Recommendation Assistant



LDCW6123 Group Project, Part 2 (Interactive Program).

Inspired by Netflix, the disruptive innovation traced in Part 1.



\## What it does

The program asks the user for their preferences and recommends a movie

from a built-in catalogue of 14 titles.



\*\*Inputs:\*\* genre (1-7), mood (1-3), age group (1-4), minutes available (60-300)



\*\*Outputs:\*\* a best-match title with a short description, a runner-up,

or a friendly "no match" message if nothing fits.



\## How it works

\- `switch` converts the genre number into its name

\- `if/else` converts the age group into the maximum age rating allowed

\- A scoring system ranks movies: genre match = 3 points, mood match = 2 points

\- Movies that are too mature or too long are filtered out

\- Invalid input is rejected and the user is asked again



\## Build and run

