#pragma once

// Putting all of the NPC AI functions in here because there are a LOT of them

namespace csvanilla
{

struct NPCHAR;
typedef void(*NPCFUNCTION)(NPCHAR*);

const NPCFUNCTION ActNpcFn[361] = {
	reinterpret_cast<NPCFUNCTION>(0x426530), // 000
	reinterpret_cast<NPCFUNCTION>(0x4265B0), // 001
	reinterpret_cast<NPCFUNCTION>(0x426AF0), // 002
	reinterpret_cast<NPCFUNCTION>(0x426FD0), // 003
	reinterpret_cast<NPCFUNCTION>(0x427040), // 004
	reinterpret_cast<NPCFUNCTION>(0x427480), // 005
	reinterpret_cast<NPCFUNCTION>(0x427820), // 006
	reinterpret_cast<NPCFUNCTION>(0x427C60), // 007
	reinterpret_cast<NPCFUNCTION>(0x427F00), // 008
	reinterpret_cast<NPCFUNCTION>(0x428260), // 009
	reinterpret_cast<NPCFUNCTION>(0x428540), // 010
	reinterpret_cast<NPCFUNCTION>(0x4289B0), // 011
	reinterpret_cast<NPCFUNCTION>(0x428B10), // 012
	reinterpret_cast<NPCFUNCTION>(0x429940), // 013
	reinterpret_cast<NPCFUNCTION>(0x429A30), // 014
	reinterpret_cast<NPCFUNCTION>(0x429BF0), // 015
	reinterpret_cast<NPCFUNCTION>(0x429E00), // 016
	reinterpret_cast<NPCFUNCTION>(0x42A0B0), // 017
	reinterpret_cast<NPCFUNCTION>(0x42A360), // 018
	reinterpret_cast<NPCFUNCTION>(0x42A490), // 019
	reinterpret_cast<NPCFUNCTION>(0x42A830), // 020
	reinterpret_cast<NPCFUNCTION>(0x42A940), // 021
	reinterpret_cast<NPCFUNCTION>(0x42A9C0), // 022
	reinterpret_cast<NPCFUNCTION>(0x42AA70), // 023
	reinterpret_cast<NPCFUNCTION>(0x42ABD0), // 024
	reinterpret_cast<NPCFUNCTION>(0x42B280), // 025
	reinterpret_cast<NPCFUNCTION>(0x42B5E0), // 026
	reinterpret_cast<NPCFUNCTION>(0x42BA90), // 027
	reinterpret_cast<NPCFUNCTION>(0x42BAE0), // 028
	reinterpret_cast<NPCFUNCTION>(0x42C1A0), // 029
	reinterpret_cast<NPCFUNCTION>(0x42C320), // 030
	reinterpret_cast<NPCFUNCTION>(0x42C4C0), // 031
	reinterpret_cast<NPCFUNCTION>(0x42CA10), // 032
	reinterpret_cast<NPCFUNCTION>(0x42CAC0), // 033
	reinterpret_cast<NPCFUNCTION>(0x42CC20), // 034
	reinterpret_cast<NPCFUNCTION>(0x42CCB0), // 035
	reinterpret_cast<NPCFUNCTION>(0x42D010), // 036
	reinterpret_cast<NPCFUNCTION>(0x42D760), // 037
	reinterpret_cast<NPCFUNCTION>(0x42D810), // 038
	reinterpret_cast<NPCFUNCTION>(0x42D960), // 039
	reinterpret_cast<NPCFUNCTION>(0x42D9F0), // 040
	reinterpret_cast<NPCFUNCTION>(0x42DE00), // 041
	reinterpret_cast<NPCFUNCTION>(0x42DE70), // 042
	reinterpret_cast<NPCFUNCTION>(0x42E9F0), // 043
	reinterpret_cast<NPCFUNCTION>(0x42EAB0), // 044
	reinterpret_cast<NPCFUNCTION>(0x42F060), // 045
	reinterpret_cast<NPCFUNCTION>(0x42F320), // 046
	reinterpret_cast<NPCFUNCTION>(0x42F3F0), // 047
	reinterpret_cast<NPCFUNCTION>(0x42F780), // 048
	reinterpret_cast<NPCFUNCTION>(0x42F9E0), // 049
	reinterpret_cast<NPCFUNCTION>(0x42FEC0), // 050
	reinterpret_cast<NPCFUNCTION>(0x4301B0), // 051
	reinterpret_cast<NPCFUNCTION>(0x430780), // 052
	reinterpret_cast<NPCFUNCTION>(0x4307D0), // 053
	reinterpret_cast<NPCFUNCTION>(0x430B00), // 054
	reinterpret_cast<NPCFUNCTION>(0x430EB0), // 055
	reinterpret_cast<NPCFUNCTION>(0x4311D0), // 056
	reinterpret_cast<NPCFUNCTION>(0x4315E0), // 057
	reinterpret_cast<NPCFUNCTION>(0x431C20), // 058
	reinterpret_cast<NPCFUNCTION>(0x4321F0), // 059
	reinterpret_cast<NPCFUNCTION>(0x432460), // 060
	reinterpret_cast<NPCFUNCTION>(0x432B50), // 061
	reinterpret_cast<NPCFUNCTION>(0x4334C0), // 062
	reinterpret_cast<NPCFUNCTION>(0x4336C0), // 063
	reinterpret_cast<NPCFUNCTION>(0x433C00), // 064
	reinterpret_cast<NPCFUNCTION>(0x433FC0), // 065
	reinterpret_cast<NPCFUNCTION>(0x4342B0), // 066
	reinterpret_cast<NPCFUNCTION>(0x4345E0), // 067
	reinterpret_cast<NPCFUNCTION>(0x434D10), // 068
	reinterpret_cast<NPCFUNCTION>(0x4355F0), // 069
	reinterpret_cast<NPCFUNCTION>(0x435AB0), // 070
	reinterpret_cast<NPCFUNCTION>(0x435BA0), // 071
	reinterpret_cast<NPCFUNCTION>(0x435DE0), // 072
	reinterpret_cast<NPCFUNCTION>(0x435FC0), // 073
	reinterpret_cast<NPCFUNCTION>(0x436180), // 074
	reinterpret_cast<NPCFUNCTION>(0x436540), // 075
	reinterpret_cast<NPCFUNCTION>(0x436650), // 076
	reinterpret_cast<NPCFUNCTION>(0x436690), // 077
	reinterpret_cast<NPCFUNCTION>(0x4367E0), // 078
	reinterpret_cast<NPCFUNCTION>(0x436870), // 079
	reinterpret_cast<NPCFUNCTION>(0x436AE0), // 080
	reinterpret_cast<NPCFUNCTION>(0x4370F0), // 081
	reinterpret_cast<NPCFUNCTION>(0x4375E0), // 082
	reinterpret_cast<NPCFUNCTION>(0x437D90), // 083
	reinterpret_cast<NPCFUNCTION>(0x438250), // 084
	reinterpret_cast<NPCFUNCTION>(0x4383D0), // 085
	reinterpret_cast<NPCFUNCTION>(0x438590), // 086
	reinterpret_cast<NPCFUNCTION>(0x438850), // 087
	reinterpret_cast<NPCFUNCTION>(0x438B10), // 088
	reinterpret_cast<NPCFUNCTION>(0x439580), // 089
	reinterpret_cast<NPCFUNCTION>(0x439B00), // 090
	reinterpret_cast<NPCFUNCTION>(0x439B50), // 091
	reinterpret_cast<NPCFUNCTION>(0x439BC0), // 092
	reinterpret_cast<NPCFUNCTION>(0x439DC0), // 093
	reinterpret_cast<NPCFUNCTION>(0x43A220), // 094
	reinterpret_cast<NPCFUNCTION>(0x43A680), // 095
	reinterpret_cast<NPCFUNCTION>(0x43AAF0), // 096
	reinterpret_cast<NPCFUNCTION>(0x43AD10), // 097
	reinterpret_cast<NPCFUNCTION>(0x43AF20), // 098
	reinterpret_cast<NPCFUNCTION>(0x43B140), // 099
	reinterpret_cast<NPCFUNCTION>(0x43B350), // 100
	reinterpret_cast<NPCFUNCTION>(0x43B410), // 101
	reinterpret_cast<NPCFUNCTION>(0x43B4E0), // 102
	reinterpret_cast<NPCFUNCTION>(0x43B5F0), // 103
	reinterpret_cast<NPCFUNCTION>(0x43B7F0), // 104
	reinterpret_cast<NPCFUNCTION>(0x43BD00), // 105
	reinterpret_cast<NPCFUNCTION>(0x43BDB0), // 106
	reinterpret_cast<NPCFUNCTION>(0x43BE00), // 107
	reinterpret_cast<NPCFUNCTION>(0x43C4B0), // 108
	reinterpret_cast<NPCFUNCTION>(0x43C610), // 109
	reinterpret_cast<NPCFUNCTION>(0x43C8E0), // 110
	reinterpret_cast<NPCFUNCTION>(0x43CDE0), // 111
	reinterpret_cast<NPCFUNCTION>(0x43D0A0), // 112
	reinterpret_cast<NPCFUNCTION>(0x43D320), // 113
	reinterpret_cast<NPCFUNCTION>(0x43D860), // 114
	reinterpret_cast<NPCFUNCTION>(0x43DAE0), // 115
	reinterpret_cast<NPCFUNCTION>(0x43E190), // 116
	reinterpret_cast<NPCFUNCTION>(0x43E1E0), // 117
	reinterpret_cast<NPCFUNCTION>(0x43E9B0), // 118
	reinterpret_cast<NPCFUNCTION>(0x43F230), // 119
	reinterpret_cast<NPCFUNCTION>(0x43F280), // 120
	reinterpret_cast<NPCFUNCTION>(0x43F310), // 121
	reinterpret_cast<NPCFUNCTION>(0x43F4A0), // 122
	reinterpret_cast<NPCFUNCTION>(0x43FC70), // 123
	reinterpret_cast<NPCFUNCTION>(0x43FEF0), // 124
	reinterpret_cast<NPCFUNCTION>(0x4400D0), // 125
	reinterpret_cast<NPCFUNCTION>(0x4401F0), // 126
	reinterpret_cast<NPCFUNCTION>(0x440760), // 127
	reinterpret_cast<NPCFUNCTION>(0x4408B0), // 128
	reinterpret_cast<NPCFUNCTION>(0x440CF0), // 129
	reinterpret_cast<NPCFUNCTION>(0x441000), // 130
	reinterpret_cast<NPCFUNCTION>(0x441360), // 131
	reinterpret_cast<NPCFUNCTION>(0x441440), // 132
	reinterpret_cast<NPCFUNCTION>(0x4419B0), // 133
	reinterpret_cast<NPCFUNCTION>(0x441B20), // 134
	reinterpret_cast<NPCFUNCTION>(0x441EC0), // 135
	reinterpret_cast<NPCFUNCTION>(0x442340), // 136
	reinterpret_cast<NPCFUNCTION>(0x442540), // 137
	reinterpret_cast<NPCFUNCTION>(0x442590), // 138
	reinterpret_cast<NPCFUNCTION>(0x442790), // 139
	reinterpret_cast<NPCFUNCTION>(0x442BF0), // 140
	reinterpret_cast<NPCFUNCTION>(0x443AC0), // 141
	reinterpret_cast<NPCFUNCTION>(0x443EC0), // 142
	reinterpret_cast<NPCFUNCTION>(0x444190), // 143
	reinterpret_cast<NPCFUNCTION>(0x444230), // 144
	reinterpret_cast<NPCFUNCTION>(0x444620), // 145
	reinterpret_cast<NPCFUNCTION>(0x444780), // 146
	reinterpret_cast<NPCFUNCTION>(0x444930), // 147
	reinterpret_cast<NPCFUNCTION>(0x445050), // 148
	reinterpret_cast<NPCFUNCTION>(0x445170), // 149
	reinterpret_cast<NPCFUNCTION>(0x445660), // 150
	reinterpret_cast<NPCFUNCTION>(0x445E30), // 151
	reinterpret_cast<NPCFUNCTION>(0x445FA0), // 152
	reinterpret_cast<NPCFUNCTION>(0x446020), // 153
	reinterpret_cast<NPCFUNCTION>(0x446500), // 154
	reinterpret_cast<NPCFUNCTION>(0x446710), // 155
	reinterpret_cast<NPCFUNCTION>(0x446B60), // 156
	reinterpret_cast<NPCFUNCTION>(0x446CA0), // 157
	reinterpret_cast<NPCFUNCTION>(0x447180), // 158
	reinterpret_cast<NPCFUNCTION>(0x4474C0), // 159
	reinterpret_cast<NPCFUNCTION>(0x447700), // 160
	reinterpret_cast<NPCFUNCTION>(0x447CB0), // 161
	reinterpret_cast<NPCFUNCTION>(0x447E90), // 162
	reinterpret_cast<NPCFUNCTION>(0x4482A0), // 163
	reinterpret_cast<NPCFUNCTION>(0x448410), // 164
	reinterpret_cast<NPCFUNCTION>(0x448580), // 165
	reinterpret_cast<NPCFUNCTION>(0x4486E0), // 166
	reinterpret_cast<NPCFUNCTION>(0x4487F0), // 167
	reinterpret_cast<NPCFUNCTION>(0x448A10), // 168
	reinterpret_cast<NPCFUNCTION>(0x448BE0), // 169
	reinterpret_cast<NPCFUNCTION>(0x4495A0), // 170
	reinterpret_cast<NPCFUNCTION>(0x4498C0), // 171
	reinterpret_cast<NPCFUNCTION>(0x449C10), // 172
	reinterpret_cast<NPCFUNCTION>(0x449D70), // 173
	reinterpret_cast<NPCFUNCTION>(0x44A3C0), // 174
	reinterpret_cast<NPCFUNCTION>(0x44A610), // 175
	reinterpret_cast<NPCFUNCTION>(0x44A7D0), // 176
	reinterpret_cast<NPCFUNCTION>(0x44ABB0), // 177
	reinterpret_cast<NPCFUNCTION>(0x44AEE0), // 178
	reinterpret_cast<NPCFUNCTION>(0x44B080), // 179
	reinterpret_cast<NPCFUNCTION>(0x44B210), // 180
	reinterpret_cast<NPCFUNCTION>(0x44BE10), // 181
	reinterpret_cast<NPCFUNCTION>(0x44C220), // 182
	reinterpret_cast<NPCFUNCTION>(0x44C630), // 183
	reinterpret_cast<NPCFUNCTION>(0x44C7A0), // 184
	reinterpret_cast<NPCFUNCTION>(0x44CA60), // 185
	reinterpret_cast<NPCFUNCTION>(0x44CBE0), // 186
	reinterpret_cast<NPCFUNCTION>(0x44CDB0), // 187
	reinterpret_cast<NPCFUNCTION>(0x44D070), // 188
	reinterpret_cast<NPCFUNCTION>(0x44D3A0), // 189
	reinterpret_cast<NPCFUNCTION>(0x44D5E0), // 190
	reinterpret_cast<NPCFUNCTION>(0x44D740), // 191
	reinterpret_cast<NPCFUNCTION>(0x44DA00), // 192
	reinterpret_cast<NPCFUNCTION>(0x44DE20), // 193
	reinterpret_cast<NPCFUNCTION>(0x44DEA0), // 194
	reinterpret_cast<NPCFUNCTION>(0x44DF10), // 195
	reinterpret_cast<NPCFUNCTION>(0x44DF60), // 196
	reinterpret_cast<NPCFUNCTION>(0x44E020), // 197
	reinterpret_cast<NPCFUNCTION>(0x44E260), // 198
	reinterpret_cast<NPCFUNCTION>(0x44E400), // 199
	reinterpret_cast<NPCFUNCTION>(0x44E5F0), // 200
	reinterpret_cast<NPCFUNCTION>(0x44EC40), // 201
	reinterpret_cast<NPCFUNCTION>(0x44ECE0), // 202
	reinterpret_cast<NPCFUNCTION>(0x44EE40), // 203
	reinterpret_cast<NPCFUNCTION>(0x44F1F0), // 204
	reinterpret_cast<NPCFUNCTION>(0x44F3E0), // 205
	reinterpret_cast<NPCFUNCTION>(0x44F6D0), // 206
	reinterpret_cast<NPCFUNCTION>(0x44FB40), // 207
	reinterpret_cast<NPCFUNCTION>(0x44FCB0), // 208
	reinterpret_cast<NPCFUNCTION>(0x450280), // 209
	reinterpret_cast<NPCFUNCTION>(0x450400), // 210
	reinterpret_cast<NPCFUNCTION>(0x450760), // 211
	reinterpret_cast<NPCFUNCTION>(0x450810), // 212
	reinterpret_cast<NPCFUNCTION>(0x450BF0), // 213
	reinterpret_cast<NPCFUNCTION>(0x4512A0), // 214
	reinterpret_cast<NPCFUNCTION>(0x451430), // 215
	reinterpret_cast<NPCFUNCTION>(0x4517F0), // 216
	reinterpret_cast<NPCFUNCTION>(0x451840), // 217
	reinterpret_cast<NPCFUNCTION>(0x451CA0), // 218
	reinterpret_cast<NPCFUNCTION>(0x451DA0), // 219
	reinterpret_cast<NPCFUNCTION>(0x451E90), // 220
	reinterpret_cast<NPCFUNCTION>(0x452000), // 221
	reinterpret_cast<NPCFUNCTION>(0x452470), // 222
	reinterpret_cast<NPCFUNCTION>(0x4524E0), // 223
	reinterpret_cast<NPCFUNCTION>(0x452700), // 224
	reinterpret_cast<NPCFUNCTION>(0x4528D0), // 225
	reinterpret_cast<NPCFUNCTION>(0x452A50), // 226
	reinterpret_cast<NPCFUNCTION>(0x452D10), // 227
	reinterpret_cast<NPCFUNCTION>(0x452D60), // 228
	reinterpret_cast<NPCFUNCTION>(0x4530D0), // 229
	reinterpret_cast<NPCFUNCTION>(0x453190), // 230
	reinterpret_cast<NPCFUNCTION>(0x453260), // 231
	reinterpret_cast<NPCFUNCTION>(0x4536F0), // 232
	reinterpret_cast<NPCFUNCTION>(0x4539B0), // 233
	reinterpret_cast<NPCFUNCTION>(0x453E60), // 234
	reinterpret_cast<NPCFUNCTION>(0x453F20), // 235
	reinterpret_cast<NPCFUNCTION>(0x454310), // 236
	reinterpret_cast<NPCFUNCTION>(0x4548B0), // 237
	reinterpret_cast<NPCFUNCTION>(0x454A00), // 238
	reinterpret_cast<NPCFUNCTION>(0x454DF0), // 239
	reinterpret_cast<NPCFUNCTION>(0x454F00), // 240
	reinterpret_cast<NPCFUNCTION>(0x455370), // 241
	reinterpret_cast<NPCFUNCTION>(0x455710), // 242
	reinterpret_cast<NPCFUNCTION>(0x455A10), // 243
	reinterpret_cast<NPCFUNCTION>(0x455AB0), // 244
	reinterpret_cast<NPCFUNCTION>(0x455C10), // 245
	reinterpret_cast<NPCFUNCTION>(0x455E00), // 246
	reinterpret_cast<NPCFUNCTION>(0x456110), // 247
	reinterpret_cast<NPCFUNCTION>(0x456F50), // 248
	reinterpret_cast<NPCFUNCTION>(0x4570B0), // 249
	reinterpret_cast<NPCFUNCTION>(0x457180), // 250
	reinterpret_cast<NPCFUNCTION>(0x457470), // 251
	reinterpret_cast<NPCFUNCTION>(0x457570), // 252
	reinterpret_cast<NPCFUNCTION>(0x4579D0), // 253
	reinterpret_cast<NPCFUNCTION>(0x457B00), // 254
	reinterpret_cast<NPCFUNCTION>(0x457D70), // 255
	reinterpret_cast<NPCFUNCTION>(0x458010), // 256
	reinterpret_cast<NPCFUNCTION>(0x458360), // 257
	reinterpret_cast<NPCFUNCTION>(0x4585A0), // 258
	reinterpret_cast<NPCFUNCTION>(0x4585F0), // 259
	reinterpret_cast<NPCFUNCTION>(0x458810), // 260
	reinterpret_cast<NPCFUNCTION>(0x458A70), // 261
	reinterpret_cast<NPCFUNCTION>(0x458C30), // 262
	reinterpret_cast<NPCFUNCTION>(0x458DF0), // 263
	reinterpret_cast<NPCFUNCTION>(0x459950), // 264
	reinterpret_cast<NPCFUNCTION>(0x459B30), // 265
	reinterpret_cast<NPCFUNCTION>(0x459C00), // 266
	reinterpret_cast<NPCFUNCTION>(0x459D80), // 267
	reinterpret_cast<NPCFUNCTION>(0x45B3D0), // 268
	reinterpret_cast<NPCFUNCTION>(0x45BCB0), // 269
	reinterpret_cast<NPCFUNCTION>(0x45BF10), // 270
	reinterpret_cast<NPCFUNCTION>(0x45C230), // 271
	reinterpret_cast<NPCFUNCTION>(0x45C500), // 272
	reinterpret_cast<NPCFUNCTION>(0x45C5A0), // 273
	reinterpret_cast<NPCFUNCTION>(0x45C750), // 274
	reinterpret_cast<NPCFUNCTION>(0x45CC80), // 275
	reinterpret_cast<NPCFUNCTION>(0x45CEA0), // 276
	reinterpret_cast<NPCFUNCTION>(0x45D780), // 277
	reinterpret_cast<NPCFUNCTION>(0x45D930), // 278
	reinterpret_cast<NPCFUNCTION>(0x45DCF0), // 279
	reinterpret_cast<NPCFUNCTION>(0x45E110), // 280
	reinterpret_cast<NPCFUNCTION>(0x45E360), // 281
	reinterpret_cast<NPCFUNCTION>(0x45E4C0), // 282
	reinterpret_cast<NPCFUNCTION>(0x45E950), // 283
	reinterpret_cast<NPCFUNCTION>(0x45F910), // 284
	reinterpret_cast<NPCFUNCTION>(0x460910), // 285
	reinterpret_cast<NPCFUNCTION>(0x460AE0), // 286
	reinterpret_cast<NPCFUNCTION>(0x460BB0), // 287
	reinterpret_cast<NPCFUNCTION>(0x460D70), // 288
	reinterpret_cast<NPCFUNCTION>(0x4610D0), // 289
	reinterpret_cast<NPCFUNCTION>(0x4614A0), // 290
	reinterpret_cast<NPCFUNCTION>(0x461800), // 291
	reinterpret_cast<NPCFUNCTION>(0x4618B0), // 292
	reinterpret_cast<NPCFUNCTION>(0x4618C0), // 293
	reinterpret_cast<NPCFUNCTION>(0x4619E0), // 294
	reinterpret_cast<NPCFUNCTION>(0x461B90), // 295
	reinterpret_cast<NPCFUNCTION>(0x461E40), // 296
	reinterpret_cast<NPCFUNCTION>(0x461FD0), // 297
	reinterpret_cast<NPCFUNCTION>(0x462050), // 298
	reinterpret_cast<NPCFUNCTION>(0x4623D0), // 299
	reinterpret_cast<NPCFUNCTION>(0x4624E0), // 300
	reinterpret_cast<NPCFUNCTION>(0x4625A0), // 301
	reinterpret_cast<NPCFUNCTION>(0x462890), // 302
	reinterpret_cast<NPCFUNCTION>(0x462AF0), // 303
	reinterpret_cast<NPCFUNCTION>(0x462C80), // 304
	reinterpret_cast<NPCFUNCTION>(0x462E00), // 305
	reinterpret_cast<NPCFUNCTION>(0x462F60), // 306
	reinterpret_cast<NPCFUNCTION>(0x4630F0), // 307
	reinterpret_cast<NPCFUNCTION>(0x4632B0), // 308
	reinterpret_cast<NPCFUNCTION>(0x463710), // 309
	reinterpret_cast<NPCFUNCTION>(0x463AC0), // 310
	reinterpret_cast<NPCFUNCTION>(0x464090), // 311
	reinterpret_cast<NPCFUNCTION>(0x464740), // 312
	reinterpret_cast<NPCFUNCTION>(0x464BB0), // 313
	reinterpret_cast<NPCFUNCTION>(0x465CC0), // 314
	reinterpret_cast<NPCFUNCTION>(0x465F60), // 315
	reinterpret_cast<NPCFUNCTION>(0x4664B0), // 316
	reinterpret_cast<NPCFUNCTION>(0x466790), // 317
	reinterpret_cast<NPCFUNCTION>(0x466B80), // 318
	reinterpret_cast<NPCFUNCTION>(0x466E50), // 319
	reinterpret_cast<NPCFUNCTION>(0x4670C0), // 320
	reinterpret_cast<NPCFUNCTION>(0x4673F0), // 321
	reinterpret_cast<NPCFUNCTION>(0x4676D0), // 322
	reinterpret_cast<NPCFUNCTION>(0x467C60), // 323
	reinterpret_cast<NPCFUNCTION>(0x467F40), // 324
	reinterpret_cast<NPCFUNCTION>(0x467FE0), // 325
	reinterpret_cast<NPCFUNCTION>(0x468230), // 326
	reinterpret_cast<NPCFUNCTION>(0x468830), // 327
	reinterpret_cast<NPCFUNCTION>(0x468990), // 328
	reinterpret_cast<NPCFUNCTION>(0x4689E0), // 329
	reinterpret_cast<NPCFUNCTION>(0x468A90), // 330
	reinterpret_cast<NPCFUNCTION>(0x468D70), // 331
	reinterpret_cast<NPCFUNCTION>(0x468F50), // 332
	reinterpret_cast<NPCFUNCTION>(0x469140), // 333
	reinterpret_cast<NPCFUNCTION>(0x469290), // 334
	reinterpret_cast<NPCFUNCTION>(0x469430), // 335
	reinterpret_cast<NPCFUNCTION>(0x469610), // 336
	reinterpret_cast<NPCFUNCTION>(0x4696B0), // 337
	reinterpret_cast<NPCFUNCTION>(0x469800), // 338
	reinterpret_cast<NPCFUNCTION>(0x469AA0), // 339
	reinterpret_cast<NPCFUNCTION>(0x469B40), // 340
	reinterpret_cast<NPCFUNCTION>(0x46B240), // 341
	reinterpret_cast<NPCFUNCTION>(0x46B340), // 342
	reinterpret_cast<NPCFUNCTION>(0x46BD80), // 343
	reinterpret_cast<NPCFUNCTION>(0x46BE10), // 344
	reinterpret_cast<NPCFUNCTION>(0x46BF00), // 345
	reinterpret_cast<NPCFUNCTION>(0x46C1D0), // 346
	reinterpret_cast<NPCFUNCTION>(0x46C710), // 347
	reinterpret_cast<NPCFUNCTION>(0x46C9B0), // 348
	reinterpret_cast<NPCFUNCTION>(0x46CAC0), // 349
	reinterpret_cast<NPCFUNCTION>(0x46CB50), // 350
	reinterpret_cast<NPCFUNCTION>(0x46D340), // 351
	reinterpret_cast<NPCFUNCTION>(0x46D5D0), // 352
	reinterpret_cast<NPCFUNCTION>(0x46DBE0), // 353
	reinterpret_cast<NPCFUNCTION>(0x46E110), // 354
	reinterpret_cast<NPCFUNCTION>(0x46E280), // 355
	reinterpret_cast<NPCFUNCTION>(0x46E480), // 356
	reinterpret_cast<NPCFUNCTION>(0x46E730), // 357
	reinterpret_cast<NPCFUNCTION>(0x46E870), // 358
	reinterpret_cast<NPCFUNCTION>(0x46E9E0), // 359
	reinterpret_cast<NPCFUNCTION>(0x46EA90)  // 360
};

}
