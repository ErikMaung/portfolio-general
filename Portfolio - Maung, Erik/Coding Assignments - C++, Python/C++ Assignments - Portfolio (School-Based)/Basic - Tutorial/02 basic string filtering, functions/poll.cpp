#include <iostream>
#include <string>
#include <cctype>
#include <cassert>
using namespace std;

bool isValidUppercaseStateCode(string stateCode)  //  obtained via the CS 31 Website
{
    const string codes =
        "AL.AK.AZ.AR.CA.CO.CT.DE.FL.GA.HI.ID.IL.IN.IA.KS.KY."
        "LA.ME.MA.MD.MI.MN.MS.MO.MT.NE.NV.NH.NJ.NM.NY.NC.ND."
        "OH.OK.OR.PA.RI.SC.SD.TN.TX.UT.VT.VA.WA.WV.WI.WY";
    return (stateCode.size() == 2 &&
        stateCode.find('.') == string::npos &&  // no '.' in stateCode
        codes.find(stateCode) != string::npos);  // match found
}

bool isValidPollString(string pollData)
{
    for (size_t k = 0; k != pollData.size(); k++)  //  run through pollData to check if everything is: LETTER, NUMBER, OR COMMA
    {
        if (!(isdigit(pollData.at(k)) || isalpha(pollData.at(k)) || pollData.at(k) == ','))
            return false;
    }

    if (pollData.size() > 0) // >0 limit prevents a negative at() reference
        if (pollData.at(0) == ',' || pollData.at(pollData.size() - 1) == ',')  //  cannot begin or end with comma -> turn FALSE
            return false;
    if (pollData.size() > 1) // >1 limit prevents a negative at() reference
        if (pollData.at(pollData.size() - 2) == ',') // since every forecast has at least TWO letters, there is no case..
            return false;                            // ..in which ',' is at the second-to-last position -> turn FALSE

    string pollCaps = ""; // initialize pollCaps variable = we will rewrite the string to all caps


    for (size_t k = 0; k != pollData.size(); k++) {
        pollCaps += toupper(pollData.at(k));  //  converts string to uppercase (pollCaps = poll, Capitalized letters)
    }                                                    //  input becomes case insensitive!

    if (pollCaps == "")  //  empty string is valid automatically (we can't do anything with it anyways)
        return true;
    if (isValidUppercaseStateCode(pollCaps)) //  string containing just one state code is fine
        return true;

    if (!isValidUppercaseStateCode(pollCaps.substr(0, 2))) //  first two letters must be a state code
        return false;
    for (size_t k = 2; k < pollCaps.size() - 2; k++) {  //  commas are NOT in first, second, second-last, or last positions
        if (pollCaps.at(k) == ',' && (!isalpha(pollCaps.at(k - 1)) || !isValidUppercaseStateCode(pollCaps.substr(k + 1, 2))))
            return false; //  what follows a comma MUST be a state code - start of a new forecast after a comma

    }

    for (size_t k = 0; k < pollCaps.size() - 1; k++) {  //  check every character except final one (prevent extending beyond string)
        if (pollCaps.at(k) == ',' && pollCaps.at(k + 1) == ',')
            return false;  //  multiple commas in a row not allowed!
    }

    for (size_t k = 0; k < pollCaps.size() - 2; k++) {  //  check to make sure no three-digit numbers appear
        if (isdigit(pollCaps.at(k)) && isdigit(pollCaps.at(k + 1)) && isdigit(pollCaps.at(k + 2)))
            return false;
        if (isalpha(pollCaps.at(k)) && isalpha(pollCaps.at(k + 1)) && isalpha(pollCaps.at(k + 2)))
            return false; //  also check to make sure no triple-letter sequences appear
    }

    for (size_t k = 3; k < pollCaps.size(); k++) { // we need less than here to prevent infinite loop! index 3 = first possible party code
        if (isalpha(pollCaps.at(k)) && pollCaps.at(k - 1) != ',' && pollCaps.at(k - 2) != ',') // if it is in a party code position
            if (isalpha(pollCaps.at(k - 1))) // there cannot be two party codes in a row without a number
                return false;
    }

    if (isdigit(pollCaps.at(pollCaps.size() - 1)))  // last character in the string cannot be a number - has to be party code
        return false;

    return true;
}

int countSeats(string pollData, char party, int& seatCount) {
    
    string pollCaps = "";                     // copied over from isValidPollString - uppercase
    for (size_t k = 0; k != pollData.size(); k++) {
        pollCaps += toupper(pollData.at(k));  //  converts string to uppercase (pollCaps = poll, Capitalized letters)
    }                                         //  input becomes case insensitive!
    party = toupper(party);                   // check for uppercase version since pollCaps is all caps

    if (! isValidPollString(pollCaps)) { // return 1 for invalid poll string (AND possibly non-letter party char)
        return 1;
    }
    if (! isalpha(party)) { // return 2 for valid poll string BUT non-letter party char
        return 2;
    }

    seatCount = 0;

    string forecast = pollCaps; // we will divide up the forcast segments and analyse each one using a while loop below

    if (pollCaps == "" || pollCaps.size() == 2) {  //  seatCount defaults to zero for empty string or for single state code in poll string
        seatCount = 0;
        return 0; // exits without analysizing unnecessarily
    }

    while (forecast.find(',') != string::npos) {  // loop: breaking up and analyzing each forecast until no more commas found
        int j = 0;
        j = forecast.find(','); // find the first comma, if any
        string forecastToCount = forecast.substr(2, j - 2); // start from the first character after the state code to the character before the comma

        if (!(forecastToCount == "")) {  //  only analyze if there is more than just a state code -- else, skip the analysis and cut to the next forecast in the string

            while (forecastToCount.find(party) != string::npos) { // continually searches for party code throughout the forecast (there may be multiple!)
                int partyLoc = forecastToCount.find(party);
                if (partyLoc - 1 >= 0 && isdigit(forecastToCount.at(partyLoc - 1))) { // check that there is a digit before party code
                    seatCount += (forecastToCount.at(partyLoc - 1) - '0'); // adds one's digit value to seatCount
                }

                if (partyLoc - 2 >= 0 && isdigit(forecastToCount.at(partyLoc - 2))) { // check first that there are at least two digits before partyLoc (prevents buffer overflow error)
                    seatCount += 10 * (forecastToCount.at(partyLoc - 2) - '0'); // adds ten times the ten's digit value to seatCount, if present
                }
                forecastToCount = forecastToCount.substr(partyLoc + 1, forecastToCount.size() - partyLoc); // after adding from the party code, cut off everything at and before it and continue searching the next forecast
            }

        }

        forecast = forecast.substr(j + 1, forecast.size() - j); // snips off the forecast that was just analyzed to prepare for next iteration

    }
    
    string lastForecast = forecast.substr(2, forecast.size() - 2); // since there are no more commas (exited while loop), we need to analyze the last forecast separately here

    if (!(lastForecast == "")) {  // skip this section of code when lastForecast becomes empty string (we'll snip the entire lastForecast at the end of the while loop, leaving an empty string)

        while (lastForecast.find(party) != string::npos) { // continually searches for the party code throughout the forecast's string (there may be multiple!)
            int partyLoc = lastForecast.find(party);
            if (lastForecast.find(party) == string::npos) // if the final forecast does not have the desired party code, we're done checking the poll string
                return 0;
    
            if (partyLoc - 1 >= 0 && isdigit(lastForecast.at(partyLoc - 1))) { // check that there is a digit before party code
               seatCount += (lastForecast.at(partyLoc - 1) - '0'); // adds one's digit value to seatCount
            }

            if (partyLoc - 2 >= 0 && isdigit(lastForecast.at(partyLoc - 2))) {
                seatCount += 10 * (lastForecast.at(partyLoc - 2) - '0'); // adds ten times the ten's digit value to seatCount, if present
            }
            lastForecast = lastForecast.substr(partyLoc + 1, lastForecast.size() - partyLoc); // to get out of the loop and the if-condition, turn lastForecast into empty string (same as forecast snipping for consistency)
        }
    }

    
    return 0;
}

// MAIN ROUTINE - JUST FOR TESTING !!

int main() {
    // BELOW ARE TRUE ASSERTS for isValidPollString (true cases)

    assert(isValidPollString("NY1R")); // basic single forecast works
    assert(isValidPollString("Ny11r2D3d,cA5G33d2b")); // multiple forecasts, mixed case, multiple parties work
    assert(isValidPollString("VT,CA,NY")); // statecodes-only works
    assert(isValidPollString("VT")); // single statecode works
    assert(isValidPollString("")); // empty string works
    assert(isValidPollString("ca00D0D,ny0r0i")); // zero values work

    // BELOW ARE OPPOSITE ASSERTS for isValidPollString (false cases)

    assert(!isValidPollString("CA5R3D,Ny101R,Tx56r12d")); // triple digit number
    assert(!isValidPollString("Ca5R3D,Nyc10R,Tx56r12d")); // triple letter state code
    assert(!isValidPollString("Ca5R3D,Ny10R,Tx56r12dd")); // double letter party code
    assert(!isValidPollString("Ca5R3D,Ny10R,Tx56r12d,")); // ends with comma
    assert(!isValidPollString(",Ca5R3D,Ny10R,Tx56r12d")); // starts with comma
    assert(!isValidPollString("Ca5R3D,Ny10,Tx56r12d")); // number is missing party code
    assert(!isValidPollString("5R3D,Ny10R,Tx56r12d")); // missing state code
    assert(!isValidPollString("Ca5R3D,,Tx56r12d")); // two commas in a row
    assert(!isValidPollString("CR5R3D,Ny10R,Tx56r12d")); // first state code invalid
    assert(!isValidPollString("Ca5R3D,NX10R,Tx56r12d")); // other state code invalid
    assert(!isValidPollString("Ca5R3D,Ny10R,Tx56r12$")); // non-alphanumeric non-comma character
    assert(!isValidPollString("Ca5R3D;Ny10R;Tx56r12d")); // non-comma separators

    // BELOW ARE ASSERTS for countSeats (return 0, 1, and 2 possibilities)

    int seats; // NOTE: adapted from Project 3 spec
    seats = -999;    // checks all forecasts in the string, case-independent
    assert(countSeats("CA5R3D,ny10D,Tx56r12d", 'd', seats) == 0 && seats == 25);

    seats = -999;    // counts zero seats for a party code that isn't present
    assert(countSeats("CA5R3D,ny10R,Tx56r12d", 'z', seats) == 0 && seats == 0);

    seats = -999;    // checks digits with zeroes properly
    assert(countSeats("CA5R0D,ny00D,Tx56r01d", 'D', seats) == 0 && seats == 1);

    // in a return 1; error, seats stay unchanged (note that I didn't assign seats in this line)
    assert(countSeats("CA5R3Dny10RTx56r12d", 'd', seats) == 1 && seats == 1);

    // in a return 2; error, seats stay unchanged (note that I didn't assign seats in this line)
    assert(countSeats("CA5R3D,ny10R,Tx56r12d", '&', seats) == 2 && seats == 1);

    seats = -999;    // can handle multiple of the same party code in one forecast & adds beyond double digits
    assert(countSeats("CA5R5R,ny10R,Tx50r31r00r99r", 'R', seats) == 0 && seats == 200);

    seats = -999;    // if both statecode and party code errors, returns 1
    assert(countSeats("CA5R3Dny10RTx56r12d", '$', seats) == 1 && seats == -999);

    seats = -999;    // no party codes, multiple state codes
    assert(countSeats("CA,ny,Tx,vT", 'D', seats) == 0 && seats == 0);

    seats = -999;    // no party codes, single state code
    assert(countSeats("ca", 'D', seats) == 0 && seats == 0);

    seats = -999;    // empty string
    assert(countSeats("", 'D', seats) == 0 && seats == 0);

    seats = -999;    // party code present but all zero entries
    assert(countSeats("ca00d,ny0d,tX0d00d0d0d00d,vt00d0d0d0d00d", 'd', seats) == 0 && seats == 0);

    seats = -999;    // lowercase input party code shares letters with state codes
    assert(countSeats("RI4r,cA1C1a,al11A12A,MA00a01A0a1A", 'a', seats) == 0 && seats == 26);

    seats = -999;    // uppercase input party code shares letters with state codes
    assert(countSeats("RI4r,ca1C1a,aL11A12A,MA00a01A0a1A", 'A', seats) == 0 && seats == 26);
}