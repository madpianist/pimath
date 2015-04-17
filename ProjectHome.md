Pimath binds 100% of Imath. Type combinations (such as constructing a V3d from a V3f) are also bound.

Pimath binds the Imath API as closely as possible. Each hpp file contains a comment block
at the top, highlighting cases where the bindings differ and why. Common reasons include:

  * Making a function signature more Pythonic;
  * Removing slightly optimised functions (where the overhead of python itself would vastly outweigh the optimisation);
  * Removing API redundancy;
  * Changing misleading Imath function names (rare).

Nice things about pimath:
  * It provides integral template instantiations where possible - eg V3i;
  * It provides 'half' template instantiations where possible - eg M33h;
  * It provides to- and from-python conversion for the Imath 'half' type;
  * It spreads type instantiation over several cpp files for quick multithreaded compiling.
  * It's organised as a mirror image of Imath - it has corresponding headers, and a free function in Imath is a free function in pimath. If you know Imath, you know pimath.