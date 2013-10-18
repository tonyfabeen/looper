clean_libuv:
	cd ext/deps/libuv && \
	make clean && \
	sudo rm /usr/local/lib/libuv* && \
	sudo rm /usr/lib/libuv*

build_libuv:
	cd ext/deps/libuv && \
	sh autogen.sh && \
	sh configure && \
	make && \
	sudo make install && \
	sudo cp /usr/local/lib/libuv* /usr/lib


gem:
	gem build looper.gemspec

rm_gem:
	rm *.gem

clean_all: clean_looper clean_libuv

