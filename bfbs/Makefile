.PHONY: nothing all

nothing:

BASE ?= sample

ALLTARGETS = $(BASE)-fbcvt.c $(BASE)-fbcvt.h $(BASE)-fbcli.c $(BASE)-fbapi.c $(BASE)_flatcc.h CMakeLists.txt
INTERMED = $(BASE).bfbs $(BASE).json $(BASE)-rpc-apis.json $(BASE)-rpc-types.json

all: $(ALLTARGETS)

# to keep intermid generated files
.PRECIOUS: $(INTERMED)

JQTYPES = '[.services[].calls[]|.request.name,.response.name]|unique|{"project_name": "$(BASE)", types:[{typename:.[]}]}'
JQAPIS = '{"project_name": "$(BASE)", apis: [.services[]|{apiname: .name, verbs: [.calls[]|{verbname: .name, request: .request.name, response: .response.name }]}]}'

# depending on version of flatcc, use either flatcc or flatc
MKBFBS = flatcc --schema
#MKBFBS = flatc -b --schema

%.bfbs: %.fbs
	$(MKBFBS) $<

%.json: %.bfbs bfbs2json
	./bfbs2json $< | jq . > $@

%-rpc-types.json: %.json
	jq $(JQTYPES) < $< > $@

%-rpc-apis.json: %.json
	jq $(JQAPIS) < $< > $@

%_json_printer.h: %.fbs
	flatcc --json-printer --outfile=$@ $<

%_json_parser.h: %.fbs
	flatcc --json-parser --outfile=$@ $<

%_flatcc.h: %.fbs
	flatcc -c -a --json --outfile=$@ $<

bfbs2json: bfbs2json.c reflection_json_printer.h
	cc -o $@ $< -lflatccrt

%-fbcvt.c: %-rpc-types.json
	./fbgen-cvt $@ $<

%-fbcvt.h: %-rpc-types.json
	./fbgen-cvt $@ $<

%-fbcli.c: %-rpc-apis.json
	./fbgen-cli $@ $<

%-fbapi.c: %-rpc-apis.json
	./fbgen-api $@ $<

CMakeLists.txt: $(BASE)-rpc-apis.json
	mustach $< fb-template-$@ >> $@

clean: 
	rm -f $(INTERMED)

mrproper: clean
	rm -f $(ALLTARGETS) reflection_json_printer.h bfbs2json CMakeLists.txt