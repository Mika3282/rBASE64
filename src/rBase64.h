// ---------------------------------------------------------------------------
// Created by Abhijit Bose (boseji) on 22/02/16.
// Copyright 2016 - Under creative commons license 3.0:
//        Attribution-ShareAlike CC BY-SA
//
// This software is furnished "as is", without technical support, and with no 
// warranty, express or implied, as to its usefulness for any purpose.
//
// Thread Safe: No
// Extendable: Yes
//
// @file rBase64.h
//
// @brief 
// Library to provide the BASE64 conversion and vis-versa
// 
// @attribution
// This is based on the prior work done by Markus Sattler for ESP8266 BASE64
// implementation and Per Ejeklint for ArduinoWebsocketServer project
//
// @version API 1.1.0 - Adding Generics 20th Mar 2018
// @version API 1.0.0 - Initial API
//
//
// @author boseji - salearj@hotmail.com
// ---------------------------------------------------------------------------

#ifndef _RBASE64_H
#define _RBASE64_H

// Function to Calculate the Encoding Length from plain text length
#define RBASE64_ENC_SIZECALC(X) ((X + 2 - ((X + 2) % 3)) / 3 * 4)

// Error Code for All is Fine
#define RBASE64_STATUS_OK 0

// Error Code for Insufficient size or decoded size error.
#define RABSE64_STATUS_SIZE 1

// Error caused due to Invalid format
#define RBASE64_STATUS_FORMAT 2

/* b64_alphabet:
 *      Description: Base64 alphabet table, a mapping between integers
 *                   and base64 digits
 *      Notes: This is an extern here but is defined in Base64.c
 */
extern const char b64_alphabet[];

/* base64_encode:
 *      Description:
 *          Encode a string of characters as base64
 *      Parameters:
 *          output: the output buffer for the encoding, stores the encoded string
 *          input: the input buffer for the encoding, stores the binary to be encoded
 *          inputLen: the length of the input buffer, in bytes
 *      Return value:
 *          Returns the length of the encoded string
 *      Requirements:
 *          1. output must not be null or empty
 *          2. input must not be null
 *          3. inputLen must be greater than or equal to 0
 */
size_t rbase64_encode(char *output, char *input, size_t inputLen);

/* base64_decode:
 *      Description:
 *          Decode a base64 encoded string into bytes
 *      Parameters:
 *          output: the output buffer for the decoding,
 *                  stores the decoded binary
 *          input: the input buffer for the decoding,
 *                 stores the base64 string to be decoded
 *          inputLen: the length of the input buffer, in bytes
 *      Return value:
 *          Returns the length of the decoded string
 *      Requirements:
 *          1. output must not be null or empty
 *          2. input must not be null
 *          3. inputLen must be greater than or equal to 0
 */
size_t rbase64_decode(char *output, char *input, size_t inputLen);

/* base64_enc_len:
 *      Description:
 *          Returns the length of a base64 encoded string whose decoded
 *          form is inputLen bytes long
 *      Parameters:
 *          inputLen: the length of the decoded string
 *      Return value:
 *          The length of a base64 encoded string whose decoded form
 *          is inputLen bytes long
 *      Requirements:
 *          None
 */
size_t rbase64_enc_len(size_t inputLen);

/* base64_dec_len:
 *      Description:
 *          Returns the length of the decoded form of a
 *          base64 encoded string
 *      Parameters:
 *          input: the base64 encoded string to be measured
 *          inputLen: the length of the base64 encoded string
 *      Return value:
 *          Returns the length of the decoded form of a
 *          base64 encoded string
 *      Requirements:
 *          1. input must not be null
 *          2. input must be greater than or equal to zero
 */
size_t rbase64_dec_len(char *input, size_t inputLen);

/*
 Base 64 Generic Storage class to support Arbitrary size 
 Encoding and Decoding of BASE64
 - sz parameter determines the Plain Text Size that is possible
*/
template<size_t sz>
class rBASE64generic {
	
	private:
	// Internal Variable to store the Size of encoding buffer
	size_t MAX_BUF_SIZE = RBASE64_ENC_SIZECALC(sz);
	// Internal Buffer for Encoding and Decoding
	uint8_t buf[RBASE64_ENC_SIZECALC(sz)];
	// Error Storage
	size_t error;
	
	public:
	/**
	 * Function to Encode a Byte Array of fixed length to a Base64 String
	 *
	 * @param data Pointer to the Source Buffer
	 * @param length Source Buffer Length
	 *
	 * @return Encoded String else the '-FAIL-' string in case of Error
	 *         - Additionally the Error codes are added internally and can
	 *         be fetched using @ref getLastError() function.
	 *
	 */
	String encode(uint8_t *data, size_t length) {
	  size_t o_length = rbase64_enc_len(length);
	  String s = "-FAIL-";
	  // Default We assume there can only be Size Error
	  error = RABSE64_STATUS_SIZE;
	  // Check Size
	  if(o_length <= MAX_BUF_SIZE)
	  {
		/* Make sure that the Length is Ok for the Output */
		if(o_length == rbase64_encode((char *)buf,(char *)data,length))
		{
		  s.reserve(o_length);
		  s = String((char *)buf);
		  error = RBASE64_STATUS_OK;
		} 
	  }
	  return s;
	}
	/**
	 * Function to Encode a NULL terminated Array to a Base64 String
	 */
	String encode(const char *data) {
		return encode((uint8_t *)data, strlen(data));
	}
	/**
	 * Function to Encode String to a Base64 String
	 */
	String encode(String text) {
		return encode((uint8_t *) text.c_str(), text.length());
	}
	/**
	 * Function to Decode the Byte Array with BASE64 encoded String to Normal String
	 *
	 * @param data Pointer to the Source Buffer
	 * @param length Source Buffer Length
	 *
	 * @return Decoded String else the '-FAIL-' string in case of Error
	 *         - Additionally the Error codes are added internally and can
	 *         be fetched using @ref getLastError() function.
	 */
	String decode(uint8_t *data, size_t length) {
		size_t o_length = rbase64_dec_len((char *)data, length);
		String s = "-FAIL-";
		// Default We assume there can only be Size Error
		error = RABSE64_STATUS_SIZE;

		// Check Size
		if(o_length <= MAX_BUF_SIZE)
		{
			/* Make sure that the Length is Ok for the Output */
			if(o_length == rbase64_decode((char *)buf,(char *)data,length))
			{
			  s.reserve(o_length);
			  s = String((char *)buf);
			}
		}
		return s;
	}
	/**
	 * Function to Decode a Null terminated Array containing BASE64 
	 * to a normal String
	 */
	String decode(const char *data) {
		return decode((uint8_t *)data, strlen(data));
	}
	/**
	 * Function to Decode a String containing BASE64 to a normal String
	 */
	String decode(String text) {
		return decode((uint8_t *) text.c_str(), text.length());
	}
	
	/**
	 * Function to report the Last error that occurred
	 */
	size_t getLastError(void) {
		return error;
	}
};

extern rBASE64generic<80> rbase64;

#endif // _RBASE64_H
