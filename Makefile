build_libuv:
	cd ext/looper/libuv && \
	sh autogen.sh && \
	sh configure && \
	make

gem:
	gem build looper.gemspec

rm_gem:
	rm *.gem

clean_all: clean_looper clean_libuv

