<p>
This section documents the time API of Arcadia Ring 2.
A point in time is represented by a timestamp - an <code>Arcadia_Integer64Value</code> - and by an <code>Arcadia_PointInTime</code> object which provides access to the date and time fields of the represented moment.
The date and time fields can be interpreted in either universal time coordinated (UTC) or local time, as determined by an <code>Arcadia_TimeSpecification</code> value.
</p>

<!-- ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ -->

<section class="cxx entity typedef">

  <h1 id="Arcadia_TimeSpecification">Arcadia_TimeSpecification</h1>

  <my-signature><code>
  typedef <my-mv>implementation detail</my-mv> Arcadia_TimeSpecification
  </code></my-signature>

  <my-summary>
  The <code>Arcadia_TimeSpecification</code> type is an enumeration type determining the time specification in which the date and time fields of a point in time are represented.
  </my-summary>

</section>

<section class="cxx entity define">

  <h1 id="Arcadia_TimeSpecification_Utc">Arcadia_TimeSpecification_Utc</h1>

  <my-signature><code>
  #define Arcadia_TimeSpecification_Utc <my-mv>implementation detail</my-mv>
  </code></my-signature>

  <my-summary>
  The <code>Arcadia_TimeSpecification_Utc</code> value denotes time in the Universal Time Coordinated (UTC) time specification.
  </my-summary>

</section>

<section class="cxx entity define">

  <h1 id="Arcadia_TimeSpecification_LocalTime">Arcadia_TimeSpecification_LocalTime</h1>

  <my-signature><code>
  #define Arcadia_TimeSpecification_LocalTime <my-mv>implementation detail</my-mv>
  </code></my-signature>

  <my-summary>
  The <code>Arcadia_TimeSpecification_LocalTime</code> value denotes time in the local time specification.
  </my-summary>

</section>

<!-- ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ -->

<section class="cxx entity object">

  <h1 id="Arcadia_PointInTime">Arcadia_PointInTime</h1>

  <p><code>Arcadia_PointInTime</code> extends <code>Arcadia_Object</code>.</p>
  <p><code>Arcadia_PointInTime</code> represents a point in time. The point in time is given by a timestamp, the number of seconds since the start of the epoch.</p>

</section>

<!-- ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ -->

<section class="cxx entity procedure">

  <h1 id="Arcadia_TimeStamp_getNow">Arcadia_TimeStamp_getNow</h1>

  <my-signature><code>
  Arcadia_Integer64Value<br>
  Arcadia_TimeStamp_getNow<br>
  &nbsp;&nbsp;(<br>
  &nbsp;&nbsp;&nbsp;&nbsp;Arcadia_Thread* thread<br>
  &nbsp;&nbsp;)
  </code></my-signature>

  <my-summary>
  Get the timestamp of now. The timestamp is the number of seconds since the start of the epoch and is independent of any time specification.
  </my-summary>

  <section class="cxx parameters">
    <h1>Parameters</h1>
    <div>
      <div><a href="#">Arcadia_Thread</a>* thread</div>
      <div>A pointer to the <code>Arcadia_Thread</code> object.</div>
    </div>
  </section>

  <section class="cxx return-value">
    <h1>Return value</h1>
    <p>The timestamp of now.</p>
  </section>

</section>

<section class="cxx entity method">

  <h1 id="Arcadia_PointInTime_create">Arcadia_PointInTime_create</h1>

  <my-signature><code>
  Arcadia_PointInTime*<br>
  Arcadia_PointInTime_create<br>
  &nbsp;&nbsp;(<br>
  &nbsp;&nbsp;&nbsp;&nbsp;Arcadia_Thread* thread,<br>
  &nbsp;&nbsp;&nbsp;&nbsp;Arcadia_Integer64Value timeStamp<br>
  &nbsp;&nbsp;)
  </code></my-signature>

  <my-summary>
  Create a point in time from a timestamp.
  </my-summary>

  <section class="cxx parameters">
    <h1>Parameters</h1>
    <div>
      <div><a href="#">Arcadia_Thread</a>* thread</div>
      <div>A pointer to the <code>Arcadia_Thread</code> object.</div>
    </div>
    <div>
      <div>Arcadia_Integer64Value timeStamp</div>
      <div>The timestamp. A timestamp is the number of seconds since the start of the epoch.</div>
    </div>
  </section>

  <section class="cxx return-value">
    <h1>Return value</h1>
    <p>A pointer to the <code>Arcadia_PointInTime</code> object.</p>
  </section>

  <section class="cxx errors">
    <h1>Errors</h1>
    <div>
      <div>Arcadia_Status_AllocationFailed</div>
      <div>An allocation failed.</div>
    </div>
  </section>

</section>

<!-- ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ -->

<section class="cxx entity method">

  <h1 id="Arcadia_PointInTime_getDayOfWeek">Arcadia_PointInTime_getDayOfWeek</h1>

  <my-signature><code>
  Arcadia_Integer8Value<br>
  Arcadia_PointInTime_getDayOfWeek<br>
  &nbsp;&nbsp;(<br>
  &nbsp;&nbsp;&nbsp;&nbsp;Arcadia_Thread* thread,<br>
  &nbsp;&nbsp;&nbsp;&nbsp;Arcadia_PointInTime* self,<br>
  &nbsp;&nbsp;&nbsp;&nbsp;Arcadia_TimeSpecification timeSpecification<br>
  &nbsp;&nbsp;)
  </code></my-signature>

  <my-summary>
  Get the day of the week of this point in time.
  </my-summary>

  <section class="cxx parameters">
    <h1>Parameters</h1>
    <div>
      <div><a href="#">Arcadia_Thread</a>* thread</div>
      <div>A pointer to the <code>Arcadia_Thread</code> object.</div>
    </div>
    <div>
      <div>Arcadia_PointInTime* self</div>
      <div>A pointer to this point in time.</div>
    </div>
    <div>
      <div>Arcadia_TimeSpecification timeSpecification</div>
      <div>The time specification in which the day of the week shall be represented.</div>
    </div>
  </section>

  <section class="cxx return-value">
    <h1>Return value</h1>
    <p>The day of the week, in the range <code>[0,6]</code>. <code>0</code> denotes Sunday, <code>6</code> denotes Saturday.</p>
  </section>

  <section class="cxx errors">
    <h1>Errors</h1>
    <div>
      <div>Arcadia_Status_ArgumentValueInvalid</div>
      <div>The value of <code>timeSpecification</code> is not a value of the <code>Arcadia_TimeSpecification</code> type.</div>
    </div>
    <div>
      <div>Arcadia_Status_EnvironmentFailed</div>
      <div>The environment failed to convert the timestamp.</div>
    </div>
  </section>

</section>

<section class="cxx entity method">

  <h1 id="Arcadia_PointInTime_getDayOfMonth">Arcadia_PointInTime_getDayOfMonth</h1>

  <my-signature><code>
  Arcadia_Integer8Value<br>
  Arcadia_PointInTime_getDayOfMonth<br>
  &nbsp;&nbsp;(<br>
  &nbsp;&nbsp;&nbsp;&nbsp;Arcadia_Thread* thread,<br>
  &nbsp;&nbsp;&nbsp;&nbsp;Arcadia_PointInTime* self,<br>
  &nbsp;&nbsp;&nbsp;&nbsp;Arcadia_TimeSpecification timeSpecification<br>
  &nbsp;&nbsp;)
  </code></my-signature>

  <my-summary>
  Get the day of the month of this point in time.
  </my-summary>

  <section class="cxx parameters">
    <h1>Parameters</h1>
    <div>
      <div><a href="#">Arcadia_Thread</a>* thread</div>
      <div>A pointer to the <code>Arcadia_Thread</code> object.</div>
    </div>
    <div>
      <div>Arcadia_PointInTime* self</div>
      <div>A pointer to this point in time.</div>
    </div>
    <div>
      <div>Arcadia_TimeSpecification timeSpecification</div>
      <div>The time specification in which the day of the month shall be represented.</div>
    </div>
  </section>

  <section class="cxx return-value">
    <h1>Return value</h1>
    <p>The day of the month, in the range <code>[0,30]</code>. The day is zero based, <code>0</code> denotes the first day of the month.</p>
  </section>

  <section class="cxx errors">
    <h1>Errors</h1>
    <div>
      <div>Arcadia_Status_ArgumentValueInvalid</div>
      <div>The value of <code>timeSpecification</code> is not a value of the <code>Arcadia_TimeSpecification</code> type.</div>
    </div>
    <div>
      <div>Arcadia_Status_EnvironmentFailed</div>
      <div>The environment failed to convert the timestamp.</div>
    </div>
  </section>

</section>

<!-- ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ -->

<section class="cxx entity method">

  <h1 id="Arcadia_PointInTime_getMonth">Arcadia_PointInTime_getMonth</h1>

  <my-signature><code>
  Arcadia_Integer8Value<br>
  Arcadia_PointInTime_getMonth<br>
  &nbsp;&nbsp;(<br>
  &nbsp;&nbsp;&nbsp;&nbsp;Arcadia_Thread* thread,<br>
  &nbsp;&nbsp;&nbsp;&nbsp;Arcadia_PointInTime* self,<br>
  &nbsp;&nbsp;&nbsp;&nbsp;Arcadia_TimeSpecification timeSpecification<br>
  &nbsp;&nbsp;)
  </code></my-signature>

  <my-summary>
  Get the month of the year of this point in time.
  </my-summary>

  <section class="cxx parameters">
    <h1>Parameters</h1>
    <div>
      <div><a href="#">Arcadia_Thread</a>* thread</div>
      <div>A pointer to the <code>Arcadia_Thread</code> object.</div>
    </div>
    <div>
      <div>Arcadia_PointInTime* self</div>
      <div>A pointer to this point in time.</div>
    </div>
    <div>
      <div>Arcadia_TimeSpecification timeSpecification</div>
      <div>The time specification in which the month of the year shall be represented.</div>
    </div>
  </section>

  <section class="cxx return-value">
    <h1>Return value</h1>
    <p>The month of the year, in the range <code>[0,11]</code>. The month is zero based, <code>0</code> denotes January, <code>11</code> denotes December.</p>
  </section>

  <section class="cxx errors">
    <h1>Errors</h1>
    <div>
      <div>Arcadia_Status_ArgumentValueInvalid</div>
      <div>The value of <code>timeSpecification</code> is not a value of the <code>Arcadia_TimeSpecification</code> type.</div>
    </div>
    <div>
      <div>Arcadia_Status_EnvironmentFailed</div>
      <div>The environment failed to convert the timestamp.</div>
    </div>
  </section>

</section>

<section class="cxx entity method">

  <h1 id="Arcadia_PointInTime_getYear">Arcadia_PointInTime_getYear</h1>

  <my-signature><code>
  Arcadia_Integer32Value<br>
  Arcadia_PointInTime_getYear<br>
  &nbsp;&nbsp;(<br>
  &nbsp;&nbsp;&nbsp;&nbsp;Arcadia_Thread* thread,<br>
  &nbsp;&nbsp;&nbsp;&nbsp;Arcadia_PointInTime* self,<br>
  &nbsp;&nbsp;&nbsp;&nbsp;Arcadia_TimeSpecification timeSpecification<br>
  &nbsp;&nbsp;)
  </code></my-signature>

  <my-summary>
  Get the year of this point in time.
  </my-summary>

  <section class="cxx parameters">
    <h1>Parameters</h1>
    <div>
      <div><a href="#">Arcadia_Thread</a>* thread</div>
      <div>A pointer to the <code>Arcadia_Thread</code> object.</div>
    </div>
    <div>
      <div>Arcadia_PointInTime* self</div>
      <div>A pointer to this point in time.</div>
    </div>
    <div>
      <div>Arcadia_TimeSpecification timeSpecification</div>
      <div>The time specification in which the year shall be represented.</div>
    </div>
  </section>

  <section class="cxx return-value">
    <h1>Return value</h1>
    <p>The year. The returned value is the number of years since 1900; adding 1900 to the returned value yields the calendar year.</p>
  </section>

  <section class="cxx errors">
    <h1>Errors</h1>
    <div>
      <div>Arcadia_Status_ArgumentValueInvalid</div>
      <div>The value of <code>timeSpecification</code> is not a value of the <code>Arcadia_TimeSpecification</code> type.</div>
    </div>
    <div>
      <div>Arcadia_Status_EnvironmentFailed</div>
      <div>The environment failed to convert the timestamp.</div>
    </div>
  </section>

</section>