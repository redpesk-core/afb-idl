# DIRECTORY BFBS

(BFBS stands for Binary Flat Buffer Schema)

This directory contains programs used to convert a FBS (flatbuffer schema)
in codes for bindings.

It uses `flatcc` from [flatcc github](https://github.com/dvidelabs/flatcc).
But be carefull, the version used here is the version 0.6.1-pre that includes
the fixes of issues #181 and #182.

It also uses `mustach` from [mustach gitlab](https://gitlab.com/jobol/mustach).

## generate code

Code is generated from JSON inputs defining types and RPC calls.

Here an example of JSON file defining types:

```json
{
  "types": [
    {
      "typename": "get_in"
    },
    {
      "typename": "get_out"
    },
    {
      "typename": "set_in"
    },
    {
      "typename": "set_out"
    },
    {
      "typename": "tes"
    }
  ]
}
```

Here an example of JSON file defining RPC alls API:

```json
{
  "apis": [
    {
      "apiname": "sample",
      "verbs": [
        {
          "verbname": "set",
          "request": "set_in",
          "response": "set_out"
        },
        {
          "verbname": "get",
          "request": "get_in",
          "response": "get_out"
        },
        {
          "verbname": "gset",
          "request": "set_in",
          "response": "tes"
        }
      ]
    }
  ]
}
```

The code is generated using mustach templates applied to these
JSON definitions. This is achieved by the tiny script `fbgen.sh`
(TO BE DOCUMENTED)

The file `Makefile` describes the implemented process.


## generate code from schema

It can be useful to generate code automaticaly based on some schema definition.
To achieve it, the tool `bfbs2json` can be used. It takes a
**binary flatbuffer schema** at its input and produce its JSON representation.

Here is the flowchart of producing `bfbs2json`.

```mermaid
graph
    B2J((bfbs2json))
    RS[reflection.fbs]
    RH(reflection_json_printer.h)
    B2JC[bfbs2json.c]
    RS -->|flatcc --json-printer| RH
    RH -->|#include| B2JC
    B2JC -->|cc -lflatccrt| B2J
```

The below flow chart shows the process of generating type and api files based on
flatbuffers schemea using `flatcc` and `bfbs2json`.

```mermaid
graph
    XS[X.fbs]
    XB(X.bfbs)
    XJ(X.json)
    XH(X_*.h)
    JEXTR(X-rpc.json)
    XS -->|flatcc --schema| XB
    XB -->|bfbs2json| XJ
    XS -->|flatcc -a --json| XH
    XJ -->|jfbsextr ...| JEXTR
```

The Makefile can be used to create these items:

```sh
$ make X-rpc-types.json X-rpc-apis.json
```


# GARBAGE

```mermaid
flowchart
    subgraph converter
        TCVT[template-cvt.c]
        MCVT((mustach))
        TCVT --> MCVT
        MCVT --> CVT
    end
    CVT(cvt.c)
    subgraph gen-api
        TAPI[template-api.c]
        API(api.c)
        MAPI((mustach))
        TAPI --> MAPI
        MAPI --> API
    end
    subgraph gen-cli
        TCLI[template-cli.c]
        CLI(client.c)
        MCLI((mustach))
        TCLI --> MCLI
        MCLI --> CLI
    end
    CVT -..-o|#include| XH
    API -..-o|#include| XH
    CLI -..-o|#include| XH
    XAPI ----> MCLI
    XCVT ----> MCVT
    XAPI ----> MAPI
```

