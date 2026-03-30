    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    lwz r0, 0x0(r3)
    lwz r5, 0x8(r3)
    rlwinm. r0, r0, 0, 14, 14
    beq @L_800EC228
    lwz r5, 0x10(r5)
    @L_800EC228:
    stw r4, 0xa4(r3)
    lwz r0, 0xa4(r3)
    cmpwi r0, 0x1
    beq @L_800EC270
    bge @L_800EC294
    cmpwi r0, 0x0
    bge @L_800EC248
    b @L_800EC294
    @L_800EC248:
    lis r4, fn_800EE054@ha
    mr r3, r5
    addi r6, r4, fn_800EE054@l
    li r5, 0x64db
    li r4, 0x6
    li r7, 0x3
    li r8, 0x0
    crxor 6, 6, 6
    bl fn_801C028C
    b @L_800EC294
    @L_800EC270:
    lis r4, fn_800EE054@ha
    mr r3, r5
    addi r6, r4, fn_800EE054@l
    li r5, 0x64db
    li r4, 0x6
    li r7, 0x3
    li r8, 0x1
    crxor 6, 6, 6
    bl fn_801C028C
    @L_800EC294:
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
