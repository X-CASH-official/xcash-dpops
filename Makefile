CPP=gcc
#CFLAGS=

LDFLAGS= -lpthread

all: xcash_proof_of_stake

xcash_proof_of_stake:
	gcc -o xcash_proof_of_stake xcash_proof_of_stake.c xcash_proof_of_stake_functions_and_macros.c xcash_proof_of_stake_test.c $(CFLAGS) $(LDFLAGS)

clean:
	rm xcash_proof_of_stake