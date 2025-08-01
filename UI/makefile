# Trình biên dịch và flags
CC      := gcc
CFLAGS  := -std=c99 -Wall -Wextra

# Các object files
OBJS    := server.o

# Tên executable sau khi link
TARGET  := server

# Thư viện Winsock (bổ sung để link socket)
LIBS    := -lws2_32

.PHONY: all clean run

# Mặc định: build executable
all: $(TARGET)

# Link các object thành executable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS) $(LIBS)

# Compile từng .c thành .o
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Xóa object và executable
clean:
	rm -f $(OBJS) $(TARGET)

# Chạy: clean → build → execute
run: clean all
	./$(TARGET)
