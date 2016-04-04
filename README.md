rhymer
======

rhymer is a utility written in C++ for the purpose of finding rhymes given a dictionary of words to phonemes like the one found [here](http://svn.code.sf.net/p/cmusphinx/code/trunk/cmudict/cmudict-0.7b).

Rhyming words are defined having their tail phonemes match, starting from the last vowel phoneme.
For example, "orange" (AO1 R AH0 N JH) and "challenge" (CH AE1 L AH0 N JH).
Vowel phonemes are defined [here](http://svn.code.sf.net/p/cmusphinx/code/trunk/cmudict/cmudict-0.7b.phones).

As an option, rhymes may not match the stress value of the last vowel phoneme.
For example, "orange" (AO1 R AH0 N JH) and "sponge" (S P AH1 N JH).