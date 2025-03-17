# So you want to contribute to Chess-E?

First off, thanks for considering it! This engine started as a weekend project, but it's grown into something I'm actually pretty proud of. Here's how you can help make it even better.

## The basics

Let's keep it simple:
- Be nice to each other
- Write decent code
- Test your stuff before submitting
- Have fun with it!

## Found a bug?

Sweet, I mean... not sweet that there's a bug, but sweet that you found it! Here's what to do: (Appreciate the joke)

1. Check if someone else already reported it
2. If not, open an issue with:
   - What happened
   - What you expected to happen
   - Steps to reproduce it (this is super important!)
   - Your system info (OS, compiler, etc)
   
Screenshots, logs, or even GIFs(Avoid sending GIFs) are always appreciated!

## Want to add something new?

Awesome! Here's the process:

1. Fork the repo
2. Create a branch with a name that makes sense
3. Write your code
4. Test it (seriously, please test it)
5. Submit a PR with a clear explanation of what you did

For bigger changes, maybe open an issue first to discuss your approach. I'm open to ideas but have some opinions on the architecture.

## Code style stuff

I'm not super strict, but try to:
- Match the style of the surrounding code
- Use meaningful names (nobody likes deciphering what `x3` does)
- Comment tricky bits, but don't overdo it
- Keep functions reasonable in size

### C++ specific things

- This is a C++17 project, so feel free to use those features
- References > pointers when it makes sense
- Use `const` where appropriate
- No memory leaks please (RAII is your friend)
- Be careful with dependencies between components

## Testing

Chess engines are notoriously bug-prone, so testing is crucial:

- Add tests for new features
- Make sure existing tests still pass
- For search or eval changes, check against known positions

## Performance matters

Chess engines need to be fast. Keep these in mind:

- Profile before optimizing
- Watch your memory allocations (especially in the search)
- Think about cache-friendliness
- Balance between speed and code readability

## Chess engine specific stuff

If you're tweaking the evaluation:
- Have test positions that show the improvement
- Small, incremental changes are better than huge rewrites
- Document your reasoning (e.g., "Bishops get +5 in open positions because...")

For search changes:
- Benchmark before and after
- Be extremely careful with pruning techniques
- Consider the impact on tactical positions

## That's it!

Thanks again for contributing. This project gets better with each person who helps out.

P.S. If you're new to chess programming, check out the Computer Chess Wiki and the TalkChess forums - both are goldmines of information. 