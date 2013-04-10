#!/bin/sh



echo "1262030463 UA120000 LO1521 DUB KRK
1262040600 UA130002 LH1230 DUB FRA Passenger rebooked to Frankfurt, his wife is flying there. Her passenger id is UA120000 please seat the man together with his wife." | ./lug - - - -

echo "1262040700 UA140000 LH1230 DUB FRA Passenger's record got totally corrupted. According to the ticket it should be: UA112358 SK4086 DUB OSL It's a pity I don't have sufficient privileges to fix this." | ./lug - - - -

echo "1262040800 UA150000 LH1111 FRA OPO The flight to Lisbone was canceled. Passenger decided not to wait for the next flight, but to go to Porto." | ./lug - - - -


echo "1262030500 UA129100 TP579 FRA OPO
1262030600 UA129100 LH1230 FRA DUB Passanger gave up a trip to Porto, the weather there is bad, -10 degrees, even -12 too cold for a vacation.
1262030700 UA129100 LH1230 FRA LHR Flight to Dublin canceled." | ./lug - - - -
