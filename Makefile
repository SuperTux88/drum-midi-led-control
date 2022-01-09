compile:
	platformio -c make run

upload: compile
	platformio -c make run --target upload --upload-port=$(port)

clean:
	platformio -c make run --target clean

all: compile upload
