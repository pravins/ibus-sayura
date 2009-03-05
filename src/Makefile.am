# vim:set noet ts=4:
#
# ibus-sayura - The Sinhala Transe engine for IBus
#
# Copyright (c) 2007-2008 Huang Peng <shawn.p.huang@gmail.com>
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2, or (at your option)
# any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

AM_CFLAGS = \
	@IBUS_CFLAGS@ \
	-DPKGDATADIR=\"$(pkgdatadir)\" \
	$(NULL)
AM_LDFLAGS = \
	@IBUS_LIBS@ \
	$(NULL)

check_PROGRAMS = \
	$(NULL)

TESTS = \
	$(check_PROGRAMS) \
	$(NULL)

libexec_PROGRAMS = ibus-engine-sayura

ibus_engine_sayura_SOURCES = \
	main.c \
	engine.c \
	engine.h \
	$(NULL)
ibus_engine_sayura_CFLAGS = \
	@IBUS_CFLAGS@ \
	-DPKGDATADIR=\"$(pkgdatadir)\" \
	$(NULL)
ibus_engine_sayura_LDFLAGS = \
	@IBUS_LIBS@ \
	$(NULL)

component_DATA = \
	sayura.xml \
	$(NULL)
componentdir = @datadir@/ibus/component

EXTRA_DIST = \
	sayura.xml.in \
	$(NULL)

CLEANFILES = \
	sayura.xml \
	$(NULL)

sayura.xml: sayura.xml.in
	( \
		libexecdir=${libexecdir}; \
		pkgdatadir=${pkgdatadir}; \
		s=`cat $<`; \
		eval "echo \"$${s}\""; \
	) > $@

test: ibus-engine-sayura
	$(builddir)/ibus-engine-sayura
