#!/usr/bin/env ruby
##### Make with #####
# ! avr-gcc -mmcu=attiny85 -Os -DF_CPU=1000000 -o blink.bin blink.c
# ! avr-objcopy -j .text -j .data -O ihex blink.bin blink.hex
# ? avr-size blink.hex
# ! avrdude -c usbasp -p t85 -U flash:w:blink.hex
##### Finished! #####

name = ARGV[0].sub(/\/$/, '')
speed = (ARGV[1].to_f * 1_000_000).to_i
file = File.read("#{name}/#{name}.c")
mmcu_match = file.match(/#define microcontroller ([a-z0-9]+)/) || []
mmcu = ARGV[2] || mmcu_match[1] || 'attiny85'
avrdude_mmcu = mmcu #.sub('attiny', 't').sub('atmega', 'm')

print `cp 'nice-things.c' '#{name}/nice-things.c'`
print `avr-gcc -mmcu=#{mmcu} -Os -DF_CPU=#{speed} -o '#{name}/firmware.bin' '#{name}/#{name}.c'`
raise "failed!" unless $? == 0
print `avr-objcopy -j .text -j .data -O ihex '#{name}/firmware.bin' '#{name}/firmware.hex'`
puts "Size of program: "
print `avr-size '#{name}/firmware.hex'`
print `avrdude -c usbasp -p #{avrdude_mmcu} -U 'flash:w:#{name}/firmware.hex'`
print `rm '#{name}/nice-things.c' '#{name}/firmware.bin' '#{name}/firmware.hex'`
