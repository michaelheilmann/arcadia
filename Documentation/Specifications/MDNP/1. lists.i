<h2 id="list">1 Lists</h2>

<h2>1.1 List</h2>
<p>
A list is an ordered list of mathematical objects.
A list is written starting with an opening square bracket \([\), followd by the elements where two consecutive elments are separated by a comma \(,\), followed by a closing square bracket \(]\).
For example, \(\left[1,2,3\right]\) is the list with three elements, the first one being \(1\), the second one being \(2\), and the third one being \(3\).
The empty list is denoted by \(\ListEmpty\).
The length of the list is usually denoted by \(\ListLength{a}\).
Elements of a list can be referenced in terms of their 1-based position. If \(a\) is a list then the i-th element of that list is usually denoted by \(\ListAt{a}{i}\).
The set of lists is denoted by \(\ListType\).
</p>

<h2>1.2 Identity of Lists</h2>
<p>
The general for for the identity of two lists \(a, b \in \ListType\) is defined by:
</p>
\[
  a = b \Leftrightarrow \ListLength{a} = \ListLength{b} \wedge \forall 1 \leq i \leq \ListLength{a} : a_i = b_i
\]

<h2>1.3 Concatenation of Lists</h2>
<p>
The concatenation of two lists \(a,b \in \ListType\) is defined indictutively:
</p>

<p>Fist, the special cases in which at least one of \(a\) and \(b\) is an empty list:
<ol>
  <li>If \(b = \ListEmpty\), then \(a \circ b = a\).</li>
  <li>If \(a = \ListEmpty\), then \(a \circ b = b\).</li>
</ol>

<p>
Otherwise both \(a\) and \(b\) each have at least one element.
Their concatenation \(c\) is then defined by \(c = a \circ b\) such that
\[
  \forall 1 \leq i \leq \ListLength{a} : \ListAt{c}{i} = \ListAt{a}{i}
  \wedge
  \forall 1 \leq j \leq \ListLength{b} : \ListAt{c}{\ListLength{a} + j} = \ListAt{b}{j}
\]
</p>
