---
content_title: Resource planning
---

How much RAM do I need? This is not an easy question to answer, and there's really no perfect answer for it. You need to find out by measuring your contracts' actions and by planning accordingly based on your predictions on how fast and how much your blockchain application will grow. If your blockchain application growth is requiring more storage capacity you'll need to buy more RAM.  If it requires more actions to be executed in the 3 day window (the staking time) you need to stake more tokens for CPU bandwidth.  If your blockchain application growth means more actions will be stored on the blockchain then you also will need to expand your NET bandwidth maximum limit by staking more tokens for NET bandwidth.

*Ok, you say, but how much?*

You need to test and simulate various business scenarios that apply to your blockchain application and measure their resource usage.  Hence, the existence of the public test networks. These allow you to measure how much RAM, CPU, and NET each action consumes, and to measure worst and best case business scenarios. You can then extrapolate and build a fairly good view of your blockchain application's resource needs.

Once you have a fair idea of how your contract, blockchain application, and user base are consuming blockchain resources on a public test-net you can estimate what you'll need to start with on any EOSIO-based networks, public or private.  From that point onward, as with any other application, it is advisable to have monitors that tell you statistics and metrics about your application performance.

Of course some aspects might differ from network to network, because each network might have altered its system contracts.  The EOSIO code base is open sourced and it can be tailored to each network's requirements. You need to be aware of these differences and take them into account if this is the case with a network you're testing on.

The EOSIO community is also providing tools that can help you in this endeavor. One example is https://www.eosrp.io
Because the RAM price varies and because the CPU and NET bandwidth allocations vary too, as it is explained in the previous section, this tool can help you estimate how much of each resource you can allocate based on a specific amount of tokens and vice-versa.

Another aspect of resource planning involves making sure your contract is efficient, that is, not consuming resources unnecessarily. Therefore, it is beneficial for you to find answers to the following questions when writing your own smart contracts and blockchain applications:

  * Is your smart contract storing only the information that is necessary to be stored on a blockchain and for the rest is using alternative ways for storing data (e.g. IPFS)?
  * If you have multiple smart contracts, are they communicating between them too much via inline actions? Could some of the smart contracts be merged into one and thus eliminate the need to spawn inline actions between them, reducing the overall inline actions count and thus resource consumption?
  * Could you change your smart contracts so that your clients pay for some parts of the RAM used? Recall how originally the addressbook contract was making each new account added to the book pay for the RAM needed to store its individual data? 
  * Or conversely, are you making your clients pay too much RAM or CPU in order to access your contracts' actions, to the point where you are prohibiting their use of your smart contract? Would it be better for your blockchain application's growth and success to take on some of those costs?
