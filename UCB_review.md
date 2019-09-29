# A Review from UCB to UCT
### Motivation: stationary v.s. non-stationary 
![](./pictures/UCB_Review.png)  
As shown in the picture the local utilization we use does not have a stationary distribution. However it's interesting to find that UCT actually can deal with non-stationary scenario while UCB can not. Thus I took some time to review some relative paper and mainly want to clarify the assumptions we need to follow once and for all if we still use UCT. I start with the basic definition.
#### Multi-armed/K-armed (stochastic) bandit problem:
$\Pi_{j}$ denotes statistical populations which is specified repectively by an univariate function $f(x; \theta_{j})$ with respect to some measure $\nu(x)$ of random variable X. 
$$
\Pi_{j} \doteq f(x; \theta_{j})\quad(j = 1,2,3,...,k)\qquad where\ f(.; .)\ is\ known\ and\ \theta_{j}\ are\ unknown\ parameters\ belonging\ to\ some\ set\ \Theta.  
$$
Assume that $\int_{-\infty}^{\infty}|x|f(x;\theta_{j})d\nu(x) < \infty$, which ensures a finite expectation of sampling, and denote a sampling sequence as $S_{X}\equiv$ {$x_{1},x_{2},...,x_{n}$}.  
Then the problem can be described as:
$$
argmax_{S_{X}}S_{n} \equiv argmax_{S_{X}}\sum_{i=1}^{n}x_{i}\quad as\ n\to\infty\quad ^{[2]}
$$
The above is the most basic formulation in mathematical way which lays the foundation for two kinds of analysis perspectives: frequentist and Bayesian approach. In loading situation the samples of $X$ are the rewards or utilizations we get from choosing an item to load.  
  
Due to the unknown expectation of reward of laying each item we have to make some compromises which split the goal into two parts: compute the estimate of rewards and define a selection strategy. 

Then comes the $adaptive\ allocation\ rule\ \varphi\ ^{[2]},\ a\ sequence\ of\ random\ variables\ \{\varphi_{1},\varphi_{2},\varphi_{3},...\}\ taken\ values\ in\ the\ set\ \{1,2,3...,k\}\\such\ that\ the\ event\ \{\varphi_{n} = j\}\ is\ generated\ by\ past\ plays\ and\ rewards\ \varphi_{1},x_{1},\varphi_{2},x_{2},...,\varphi_{n-1},x_{n-1}$.  
Let $\mu(\theta)=\int_{-\infty}^{\infty}xf(x;\theta)d\nu(x)$. Then 
$$
E[S_{n}]=\sum_{j=1}^{k}\mu(\theta_{j})E[T_{n}(j)]\\where\ T_{n}(j)=\sum_{i=1}^{n}I_{\{\varphi_{i}=j\}}\ is\ the\ number\ of\ times\ that\ j\ is\ selected. 
$$
and the $regret$ of the rule:
$$
R_{n}(\theta_{1},...,\theta_{k})=\sum_{j:\ \mu(\theta_{j})<\mu^{*}}(\mu^{*}-\mu(\theta_{j}))E[T_{n}(j)]\quad where \mu^{*}\doteq max_{1\le i\le K}\mu_{i}
$$  
The research on allocation rule found that $E[T_{j}(n)]=\Theta(logn)\ ^{[2]}\ for\ any\ fixed\ values\ \theta_{1},...,\theta_{k}\ for\ which\ the\ \mu(\theta_{j})\ are\ not\ all\ equal$  
These policies work by using $upper\ confidence\ index$ as an estimate for the corresponding reward expectation.

UCB1(Upper Confidence Bound) achieves logarithmic regret uniformly over time rather than only asymptotically$^{[3]}$, which means $E[T_{j}(n)]=\alpha(logn)\quad where\  \alpha\ is\ a\ constant$, based on the assumptions: (denote $X_{i,s}\equiv the\ reward\ of\ choosing\ i\ at\ time\ s$)
$$
X_{i,1},\ X_{i,2},\ ...\ X_{i,s}\ are\ i.i.d,\ namely\ X_{i}\ has\ a\ stationary\ distribution\\X_{i,s}\ and\ X_{j,t}\ are\ independent\ for\ each\ 1\le i<j\le K\ and\ each\ s,t\ge 1\\X_{i,s}\in [0,1],\ \ 1\le i\le K,\ s\ge 1
$$ 
The main reason UCB1 can not deal with non-stationary scenario is that the proof uses $Chernoff-Hoeffding\ bound$ which describes how far the random variable is from its expectation.

UCT(Upper Confidence Bounds for Trees) adapts UCB1 in Tree Policy:
$$
every\ time\ a\ node(action)\ is\ to\ be\ selected\ within\ the\ existing\ tree\ the\ choice\ is\ modelled\ as\ an\ independent\ multi-armed\ bandit\ problem\ ^{[1]}
$$
But the difference is that the reward here is defined as the result in a lookahead simulation, which is the main feature in MCTS as to solve the search problems where heuristic methods are hard to define or quantify. Thus UCT is held under one of the few assumptions which I think is the key to explain why UCT can solve non-stationary scenario:
$$
the\ limit\ of\ \mu_{in}=E[\overline{X}_{in}]\ exists^{[4]},\ namely\ as\ the\ times\ of\ simulation\ goes\ to\ infinity\ the\ expectation\ converges.
$$
Then intuitively even if the distribution is non-stationary the estimate is always getting close to the expectation. All UCT needs to do is to set a certain constant that numerically bounds how close it would be, i.e. $C_{p}$ in the UCT formula.
(If you want to check the detail of UCT I recommend [4] instead of [5] which contains no proof)

In conclusion, if you want to refine the current MCTS algorithm following UCT be careful not to break the assumptions in UCB1 and UCT.　These assumptions are also the possible breakpoints for general R&D and I'm now turning to Bayesian approach which can be an attempt and already a popular method in Reinforcement Learning since UCB1 does not ensure a better performance than $\varepsilon -greedy$ and requires large computation.

I'll do some numerical experiments to check the convergence assumption as well as model the reward distribution which is required in Bayesian approach.

Reference:  
[1].Browne et al. - 2012 - A Survey of Monte Carlo Tree Search Methods  
[2].Lai et.al.-1985-Asymptotically efficient adaptive allocation rules  
[3].Auer - 2002 - Finite-time analysis of the multi-armed bandit problem with known trend  
[4].Kocsis et.al.-2006-Improved Monte-Carlo Search  
[5].Kocsis, Szepesvári - 2006 - Bandit Based Monte-Carlo Planningv  
[6].kaufmann et.al.-2012-On Bayesian Upper Confidence Bounds for Bandit Problems  
[7].Yasin et.al.-2011-Improved Algorithms for Linear Stochastic Bandits  
[8].Sutton, Barto -2017-Reinforcement Learning: An Introduction
