# SNAKH

Your casual snake game - except everything takes place in a:

## Hyperbolic plane

That's one of the famous non-euclidean planes. The grid layout is like the hyperbolic counterpart of Platonic solids, with every vertex connecting four others and every "face" consisting of five sides. Lines won't stay parallel. Two lines, however close they are at some point, drifts away as they extend. Our snake had better try not to get lost as it searches for those apples.

Dealing with such grids is a headache - not just out of how confusing it is. This project requires a whole lot of:

## Computational geometry

My intuition is not sharp enough to notice any special property that immediately tells me what this graph looks like. So I just turn to this simpleminded method: checking if the coordinates of two points are strictly equal in order to know whether they overlap.

*Wait, what about floating point errors??*

In fact, I've been paranoid about floating points and would avoid them wherever possible. My calculations have shown that all numbers in those coordinates can be represented by $\frac{a + b \sqrt{5}}{c}$ where $a, b, c$ are integers. By implementing a structure that handles just that, I got to throw away all those floating point numbers.

But that leaves us with a problem...

## Encouraging the player to stay near the origin

Okay. Hyperbolic planes are boundless. I know how I could have made an open-world thing. But *I'm dangerously close to the deadline*. I can think of a few solutions, but then I'll lose my race against the clock. That's a serious technical debt.

Here's the issue: far-away coordinates *grow exponentially*. You're just tens of steps away from the `long long` maximum. Beyond that, lies the absurd domain of unknown. Of chaos. Of eldritch, unspeakable horror.

Right now, apples only generate near the origin. In the meantime, all I can do is to trust my players in that, for their own sake, they know better than wandering too far away.
