#!/bin/bash
zip -r `date +%s`.zip main.c KeyInfoStream.c KeyInfoStream.h IdentifierHash.c IdentifierHash.h HashTableFast.c HashTableFast.h editdistDP.c CodeSimilarity.c CodeSimilarity.h opt.h
