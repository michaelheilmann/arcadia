<h2 id="computer-arithmetic">3 Computer Arithmetic</h2>

<h3>Natural Number</h3>
<p>Natural numbers can be represented by a sequence of bits \(x_{n-1} \ldots x_{0}, n > 0\).
To obtain the natural number it represents compute \(\sum_{i=0}^{n-1} x_{i} 2^{i}\).
For example, the number \(5\) can be represented by \(101\) that is \(1 \cdot 2^2 + 0 \cdot 2^1 + 1 \cdot 2^0 = 5\).
We can prepend zeroes to a binary string without changing the value it represents by the above formula.
For example 101, 0101, 00101, ... all represent the value 5 by the above formula.
</p>

<h3>Addition, Subtraction, and Multiplication</h3>
<ul>
  <li>Let \(x\) and \(y\) be natural number binary strings representing natural numbers \(a\) and \(b\).</li>
  <li>Let the width of \(x\) be \(n\) and the width of \(y\) be \(m\), respectively, where \(n, m &gt; 0\).</li>
  <li>Let \(k = \textit{max}\left(n, m\right)\).</li>
  <li>
  Compute
  \[
  c := a \circ b \mod 2^k
  \]
  where \(\circ\) is the operation \(+\) (for addition), \(-\) (for subtraction), and \(\circ\) (for multiplication).
  </li>
  <li>
  \(c\) is in the bounds of \([0,2^k-1]\) and hence can be represented by binary string of \(k\) bits.
  </li>
  <li>
  The result of the operation is \(z\) the binary string representation of \(c\) by \(k\) bits.
  </li>
</ul>

<h3>Division</h3>
<ul>
  <li>Let \(x\) and \(y\) be binary strings representing natural numbers \(a\) and \(b\).</li>
  <li>Let the width of \(x\) be \(n\) and the width of \(y\) be \(m\), respectively, where \(n, m &gt; 0\).</li>
  <li>Let \(k = \textit{max}\left(n, m\right)\).</li>
  <li>Perform a width-extension to \(k\) to obtain \(x'\) and \(y'\), respectively.</li>
  <li>
  Compute
  \[
  c := \textit{floor}(a / b) \mod 2^k*
  \]
  where \(/\) is the normal division and \(\textit{floor}\) is the floor function. Observe that \(mod 2^k\) is redundant.
  Furthermore this is undefined if \(b is zero\).
  \(c\) is representable by a binary string of length \(k\).
  </li>
  <li>
  The result of the operation is either undefined or \(z\) the representation of \(c\) by \(k\) bits.
  </li>
</ul>

<h3>Integer Numbers</h3>
<p>Integer numbers can represented by sequences of bits \(x_{n-1} \ldots x_{0}, n > 1\).
To obtain the integer number it represents compute \(- x_{n-1} 2^{n-1} + \sum_{i=0}^{n-2} x_i 2^i\).
</p>

<h3>Addition and Multiplication</h3>
<ul>
  <li>
  Let \(x\) and \(y\) be integer number binary strings representing integer numbers \(a\) and \(b\).
  </li>
  <li>
  Perform sign-extension of \(x\) and \(y\) to obtain \(x'\) and \(y'\), respectively.
  </li>
  <li>
  Let the width of \(x'\) be \(n\) and the width of \(y'\) be \(m\), respectively, where \(n, m &gt; 0\).
  </li>
  <li>
  Let \(k = \textit{max}\left(n, m\right)\).
  </li>
  <li>
  Compute
  \[
  c := a \circ b \mod 2^k
  \]
  where \(\circ\) is the operation \(+\) (for addition) and \(\circ\) (for multiplication).
  \(c\) is in the bounds of \([-(2^{k-1}), (2^{k-1} -1)]\) and hence can be represented by \(k\) bits.
  </li>
  <li>
  The result of the operation is \(z\) the representation of \(c\) by \(k\) bits.
  </li>
</ul>

<h3>Division</h3>
<ul>
  <li>Let \(x\) and \(y\) be integer number binary strings representing integer numbers \(a\) and \(b\).</li>
  <li>Perform sign-extension of \(x\) and \(y\) to obtain \(x'\) and \(y'\), respectively.</li>
  <li>Let the width of \(x'\) be \(n\) and the width of \(y'\) be \(m\), respectively, where \(n, m &gt; 0\).</li>
  <li>Let \(k = \textit{max}\left(n, m\right)\).</li>
  <li>
  Compute
  \[
  c := \textit{floor}(a / b) \mod 2^k
  \]
  where \(/\) is the normal division and \(\textit{floor}\) is the floor function. Observe that \(mod 2^k\) is redundant.
  Furthermore this is undefined if \(b is zero\).
  </li>
  <li>
  \(c\) is in the bounds of \([-(2^{k-1}), (2^{k-1} -1)]\) and hence can be represented by \(k\) bits.
  </li>
  <li>The result of the operation is \(z\) the representation of \(c\) by \(k\) bits.</li>
</ul>
