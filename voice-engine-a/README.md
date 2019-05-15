# Voice Engine A

License-free replacement for Google TTS Engine.
We replaced Android::Autolock with std::lock\_guard to reduce heavy dependency on AOSP.

https://android.googlesource.com/platform/external/svox/


Original dependency:

* aosp-monkey-patch
	* liblog

* libsvoxpico

* ttscompat
	* libandroid\_runtime
	* libnativehelper
	* libmedia
	* libutils
	* libcutils
	* liblog
	* libdl

* libttspico
	* libttspico\_engine (static)
		* libsvoxpico
		* libcutils, libexpat, libutils, liblog
	* libttspico (shared)
		* libsvoxpico
		* libcutils, libexpat, libutils, liblog

* external/expat



