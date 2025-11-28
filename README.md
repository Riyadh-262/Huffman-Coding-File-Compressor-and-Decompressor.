# 📦 Huffman Coding File Compressor & Decompressor (C++)

This project is a full implementation of **Huffman Coding**, a classic lossless data compression algorithm.  
It allows you to **compress any file** into a `.huf` binary format and **restore it back** without losing data.

The entire program is implemented in **pure C++**, including:
- Custom Min-Heap
- Huffman Tree Construction
- Code Generation
- Bit-level Compression
- Binary File Decompression

---

## 🚀 Features

### ✔️ Huffman-Based Compression
Builds a Huffman tree from character frequency and generates a compact bit-based encoding.

### ✔️ Lossless Decompression
Fully restores the original file by traversing the same Huffman tree.

### ✔️ Custom Min-Heap
No STL priority_queue — heap is built manually for learning purposes.

### ✔️ Binary I/O Handling
Reads and writes raw bytes for efficient compression.

### ✔️ Displays Compression Statistics
After compression:
- Original size  
- Compressed size  
- Compression ratio  

---

---

## 🧠 How It Works

### 1️⃣ Count Character Frequencies  
Reads each byte from the input file and counts frequencies of all 256 ASCII characters.

### 2️⃣ Build Huffman Tree  
Repeatedly extracts the two smallest nodes from the heap and merges them.

### 3️⃣ Generate Huffman Codes  
Traverses the Huffman tree to generate unique bit strings for each character.

### 4️⃣ Compress the Data  
Writes:
- Frequency table (256 integers)
- Encoded bitstream of original file

### 5️⃣ Decompression  
Reads frequency table → rebuilds Huffman tree → decodes bitstream back into characters.

---

## 🖥️ Usage

Compile the program:

```bash
g++ huffman.cpp -o huffman
./huffman -c input.txt output.huf
./huffman -d output.huf restored.txt

huffman.exe -c Input1.txt outputH.huf
huffman.exe -d outputH.huf RestoredH.txt



## 📁 File Structure

