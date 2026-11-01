<section>
<h2 id="dsp-reproducibility">DSP reproducibility and streaming scope</h2>
<p>
For a deterministic node or graph, identical initial state, topology, input
samples, configuration, sample rate, seeds and execution environment must produce
bitwise-identical samples on fresh-state repetitions. The environment includes
the binary, relevant compiler/FP settings, library runtime, rounding and subnormal
policy. Numeric equality is not bit equality (notably for signed zero). The
verification profile below uses finite values; NaN payloads and non-finite
inputs need explicit per-operator policy and are not covered by this promise.
</p>
<p>
For supported streaming topologies, sequential chunks at the same logical time
must concatenate to the one-shot sequence. This is buffer-partition independence,
not a cross-platform bitwise guarantee. Keep phase, PRNG and filter state across
chunks and hold sample rate/configuration fixed. Graph reset walks registered
nodes in insertion order; reseed derives a seed from the supplied seed and each
node's stored index, then resets that node. Preserve registration and Mix input
order, including duplicate entries; do not infer that floating-point accumulation
is invariant under reordering.
</p>
<table>
<thead><tr><th>Selected profile</th><th>State and verification</th></tr></thead>
<tbody>
<tr><td>SineWave, SawtoothWave</td><td>Phase retained per sample; reset restores phase zero, reseeding does not change oscillator configuration. Reproducibility checks fresh/reset and one-shot versus irregular chunks, including zero-length buffers.</td></tr>
<tr><td>WhiteNoise, BrownNoise</td><td>PRNG state retained per sample; BrownNoise also retains its accumulator. Reset restores the current initial seed (and BrownNoise accumulator zero); reseed changes that initial state. Repeated seed/reset and partition checks are required.</td></tr>
<tr><td>Independent-source tree</td><td>A fixed ordered Mix of WhiteNoise/Gain, BrownNoise/LowPass and Sine/Affine multiplied by a distinct Sawtooth source. Every stateful source is evaluated once per output block. Fresh, reset, reseed and partition checks cover the composed state.</td></tr>
<tr><td>Engine GraphDualTone</td><td>Actual ADL graph/factory/provider path: fresh and partial-chunk outputs are compared byte-for-byte separately from a numerical sine-sum reference.</td></tr>
</tbody>
</table>
<p>
This is not a claim for every possible graph. Native Graph renders its output
recursively; it is not a per-sample scheduling/cache engine. Shared or duplicated
stateful inputs are rendered once per reference, so they are excluded from the
partition-independent topology profile above pending separate scheduling/aliasing
semantics and tests. Mix's duplicate summation policy is unchanged. Composite
reset depends on registering all stateful nodes; resetting one wrapper alone
does not recursively reset arbitrary dependencies.
</p>
<p>
Across platforms, numerical equivalence needs per-operator reference vectors and
justified absolute/relative tolerances, not one universal epsilon. For example,
the 257-frame, 48 kHz, 440 Hz oscillator profile compares to double-precision
analytic references with absolute limits 1e-4 (sine) and 1e-5 (sawtooth), allowing
short-horizon Real32 phase accumulation/libm error, not arbitrary-duration drift.
WhiteNoise's reference uses exact unsigned xorshift steps and double-precision
normalization/scaling with absolute 1e-7 for Real32 arithmetic error; BrownNoise's
double-precision clamped recurrence uses absolute 2e-6 for accumulation over the
same 257-frame horizon. The mixed/filter tree currently has byte-repeat/partition
checks, not a new independent cross-platform numerical error budget.
The separate 16-frame Engine dual-tone oracle uses absolute 1e-5. These reference
checks do not establish a multi-platform pass: Windows-only execution must be
reported as such, with other platform gates outstanding.
</p>
<p>
Cross-platform bitwise identity is expressly not required. Library sinf/fmodf/expf,
FMA/reassociation, intermediate precision and FP environment may differ. Floating
point remains permitted; stricter arithmetic profiles or fixed-point rewrites
require a separate design and authorization.
</p>
</section>
