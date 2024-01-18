#include <iostream>
#include <vector>

typedef unsigned char byte;

struct IInputStream {
    // Возвращает false, если поток закончился
    virtual bool Read(byte& value) = 0;
};

struct IOutputStream {
    virtual void Write(byte value) = 0;
};


class VectorOutput : public IOutputStream {
public:
    VectorOutput(std::vector<byte>* output_buffer): buf(output_buffer) {

    }

    virtual void Write(byte value) {
        buf->push_back(value);
    }


private:
    std::vector<byte>* buf;
};


class VectorInput : public IInputStream {
public:
    VectorInput(std::vector<byte>* input_buffer ) : buf(input_buffer), pos(0) {
    }

    virtual bool Read(byte& value) {
        if (pos >= buf->size())
            return false;
        value = (*buf)[pos++];
        return true;
    }
private:
    std::vector<byte>* buf;
    size_t pos;
};




// int main(int argc, const char * argv[]) {
//     std::vector<byte> vec_output;
//     VectorOutput vo(&vec_output);
//     BitWriter bw(vo);
//     bw.write_bit(1);
//     bw.write_bit(0);
//     bw.write_bit(1);
//     bw.write_bit(0);
//     bw.write_bit(1);
//     bw.write_bit(0);
//     bw.write_bit(1);
//     bw.write_bit(0);

//     bw.write_bit(1);
//     bw.write_bit(1);
//     bw.flush();

//     VectorInput vi(&vec_output);
//     BitReader br(vi);

//     byte value = 0;
//     while (br.read_bit(value)) {
//         std::cout << (int)value;
//     }
//     std::cout << std::endl;


//     std::cout << "OK" << std::endl;
// }