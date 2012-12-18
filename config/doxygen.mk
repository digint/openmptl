DOXYGEN      = doxygen


.PHONY: doxygen

doxygen:
	$(DOXYGEN) $(DOCDIR)/doxygen.conf

