
static const char _afb_description_perms[] =
	"{\"afbidl\":\"0.1\",\"info\":{\"apiname\":\"perms\",\"description\":"
	"\"demo for permissions\",\"title\":\"demo for permissions\",\"versio"
	"n\":\"1.0\"},\"tools\":{\"genskel\":{\"version\":2,\"prefix\":\"f_\""
	",\"postfix\":\"\",\"preinit\":null,\"init\":null,\"onevent\":null,\""
	"scope\":\"static\",\"private\":true}},\"verbs\":{\"check\":{\"permis"
	"sions\":{\"session\":\"check\"}},\"loa1\":{\"permissions\":{\"LOA\":"
	"1}},\"perm1\":{\"permissions\":{\"permission\":\"permission-1\"}},\""
	"perm2\":{\"permissions\":{\"permission\":\"permission-2\"}},\"perm1o"
	"r2or3\":{\"permissions\":{\"anyOf\":[{\"permission\":\"permission-1\""
	"},{\"permission\":\"permission-2\"},{\"permission\":\"permission-3\""
	"}]}},\"perm1and2and5\":{\"permissions\":{\"allOf\":[{\"permission\":"
	"\"permission-1\"},{\"permission\":\"permission-2\"},{\"permission\":"
	"\"permission-4\"}]}},\"permXXX\":{\"permissions\":{\"allOf\":[{\"LOA"
	"\":3},{\"session\":\"check\"},{\"anyOf\":[{\"permission\":\"permissi"
	"on-1\"},{\"not\":{\"permission\":\"permission-1\"}},{\"allOf\":[{\"p"
	"ermission\":\"permission-2\"},{\"permission\":\"permission-4\"}]}]}]"
	"}}}}"
;

static const struct afb_auth _afb_auths_perms[] = {
	{ .type = afb_auth_Permission, .text = "permission-1" },
	{ .type = afb_auth_Permission, .text = "permission-2" },
	{ .type = afb_auth_Permission, .text = "permission-3" },
	{ .type = afb_auth_Or, .first = &_afb_auths_perms[1], .next = &_afb_auths_perms[2] },
	{ .type = afb_auth_Or, .first = &_afb_auths_perms[0], .next = &_afb_auths_perms[3] },
	{ .type = afb_auth_Permission, .text = "permission-4" },
	{ .type = afb_auth_And, .first = &_afb_auths_perms[1], .next = &_afb_auths_perms[5] },
	{ .type = afb_auth_And, .first = &_afb_auths_perms[0], .next = &_afb_auths_perms[6] },
	{ .type = afb_auth_Not, .first = &_afb_auths_perms[0] },
	{ .type = afb_auth_Or, .first = &_afb_auths_perms[8], .next = &_afb_auths_perms[6] },
	{ .type = afb_auth_Or, .first = &_afb_auths_perms[0], .next = &_afb_auths_perms[9] }
};

static void afb_verb_check_cb(afb_req_t req);
static void afb_verb_loa1_cb(afb_req_t req);
static void afb_verb_perm1_cb(afb_req_t req);
static void afb_verb_perm2_cb(afb_req_t req);
static void afb_verb_perm1or2or3_cb(afb_req_t req);
static void afb_verb_perm1and2and5_cb(afb_req_t req);
static void afb_verb_permXXX_cb(afb_req_t req);

static const struct afb_verb_v3 _afb_verbs_perms[] = {
    {
        .verb = "check",
        .callback = afb_verb_check_cb,
        .auth = NULL,
        .info = NULL,
        .vcbdata = NULL,
        .session = AFB_SESSION_CHECK,
        .glob = 0
    },
    {
        .verb = "loa1",
        .callback = afb_verb_loa1_cb,
        .auth = NULL,
        .info = NULL,
        .vcbdata = NULL,
        .session = AFB_SESSION_LOA_1,
        .glob = 0
    },
    {
        .verb = "perm1",
        .callback = afb_verb_perm1_cb,
        .auth = &_afb_auths_perms[0],
        .info = NULL,
        .vcbdata = NULL,
        .session = AFB_SESSION_NONE,
        .glob = 0
    },
    {
        .verb = "perm2",
        .callback = afb_verb_perm2_cb,
        .auth = &_afb_auths_perms[1],
        .info = NULL,
        .vcbdata = NULL,
        .session = AFB_SESSION_NONE,
        .glob = 0
    },
    {
        .verb = "perm1or2or3",
        .callback = afb_verb_perm1or2or3_cb,
        .auth = &_afb_auths_perms[4],
        .info = NULL,
        .vcbdata = NULL,
        .session = AFB_SESSION_NONE,
        .glob = 0
    },
    {
        .verb = "perm1and2and5",
        .callback = afb_verb_perm1and2and5_cb,
        .auth = &_afb_auths_perms[7],
        .info = NULL,
        .vcbdata = NULL,
        .session = AFB_SESSION_NONE,
        .glob = 0
    },
    {
        .verb = "permXXX",
        .callback = afb_verb_permXXX_cb,
        .auth = &_afb_auths_perms[10],
        .info = NULL,
        .vcbdata = NULL,
        .session = AFB_SESSION_CHECK|AFB_SESSION_LOA_3,
        .glob = 0
    },
    {
        .verb = NULL,
        .callback = NULL,
        .auth = NULL,
        .info = NULL,
        .vcbdata = NULL,
        .session = 0,
        .glob = 0
    }
};

const struct afb_binding_v3 afbBindingV3 = {
    .api = "perms",
    .specification = _afb_description_perms,
    .info = "demo for permissions",
    .verbs = _afb_verbs_perms,
    .preinit = NULL,
    .init = NULL,
    .onevent = NULL,
    .userdata = NULL,
    .provide_class = NULL,
    .require_class = NULL,
    .require_api = NULL,
    .noconcurrency = 0
};

