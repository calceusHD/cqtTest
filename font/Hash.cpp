#include "Hash.h"

Hash::Hash():h(4)
{
    //std::stringstream();
}

Hash::~Hash()
{
    //dtor
}

std::vector<uint32_t> Hash::digest()
{
    uint8_t *msg = nullptr;
    // Initialize variables - simple count in nibbles:
    h[0] = 0x67452301;
    h[1] = 0xefcdab89;
    h[2] = 0x98badcfe;
    h[3] = 0x10325476;

    // Pre-processing: adding a single 1 bit
    //append "1" bit to message
    /* Notice: the input bytes are considered as bits strings,
       where the first bit is the most significant bit of the byte.[37] */

    // Pre-processing: padding with zeros
    //append "0" bit until message length in bit ≡ 448 (mod 512)
    //append length mod (2 pow 64) to message

    int new_len;
    for(new_len = this->str().length()*8 + 1; new_len%512!=448; new_len++);
    new_len /= 8;

    msg = (uint8_t*)calloc(new_len + 64, 1); // also appends "0" bits
                                   // (we alloc also 64 extra bytes...)
    memcpy(msg, this->str().c_str(), this->str().length());
    msg[this->str().length()] = 128; // write the "1" bit

    uint32_t bits_len = 8*this->str().length(); // note, we append the len
    memcpy(msg + new_len, &bits_len, 4);           // in bits at the end of the buffer

    // Process the message in successive 512-bit chunks:
    //for each 512-bit chunk of message:
    int offset;
    for(offset=0; offset<new_len; offset += (512/8)) {

        // break chunk into sixteen 32-bit words w[j], 0 ≤ j ≤ 15
        uint32_t *w = (uint32_t *) (msg + offset);



        // Initialize hash value for this chunk:
        uint32_t a = h[0];
        uint32_t b = h[1];
        uint32_t c = h[2];
        uint32_t d = h[3];

        // Main loop:
        uint32_t i;
        for(i = 0; i<64; i++) {

            uint32_t f, g;

             if (i < 16) {
                f = (b & c) | ((~b) & d);
                g = i;
            } else if (i < 32) {
                f = (d & b) | ((~d) & c);
                g = (5*i + 1) % 16;
            } else if (i < 48) {
                f = b ^ c ^ d;
                g = (3*i + 5) % 16;
            } else {
                f = c ^ (b | (~d));
                g = (7*i) % 16;
            }

             uint32_t temp = d;
            d = c;
            c = b;
            b = b + LEFTROTATE((a + f + k[i] + w[g]), r[i]);
            a = temp;

        }

        // Add this chunk's hash to result so far:
        h[0] += a;
        h[1] += b;
        h[2] += c;
        h[3] += d;

    }

    // cleanup
    free(msg);

    return h;
}
