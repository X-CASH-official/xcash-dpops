CPP=gcc
#CFLAGS=

LDFLAGS= -lpthread

all: xcash_proof_of_stake_client xcash_proof_of_stake_server

xcash_proof_of_stake_client:
	gcc -o xcash_proof_of_stake_client xcash_proof_of_stake_client.c xcash_proof_of_stake_functions_and_macros.c xcash_proof_of_stake_test.c $(CFLAGS) $(LDFLAGS)

xcash_proof_of_stake_server:
	gcc -o xcash_proof_of_stake_server xcash_proof_of_stake_server.c xcash_proof_of_stake_functions_and_macros.c xcash_proof_of_stake_test.c $(CFLAGS) $(LDFLAGS)

clean:
	rm xcash_proof_of_stake_client xcash_proof_of_stake_server