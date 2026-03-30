    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    stw r30, 0x8(r1)
    mr r30, r3
    lwz r0, 0x74(r3)
    add r0, r0, r4
    stw r0, 0x74(r3)
    lwz r0, 0x4(r3)
    cmplwi r0, 0x6
    bgt @L_801D84AC
    lis r3, -32713
    rlwinm r0, r0, 2, 0, 29
    addi r3, r3, 9672
    lwzx r0, r3, r0
    mtctr r0
    bctr
    lwz r3, 0xa4(r30)
    cmplwi r3, 0x0
    beq @L_801D84D8
    lwz r0, 0x88(r30)
    cmpwi r0, 0x0
    bge @L_801D8234
    li r31, 0x1
    b @L_801D8288
@L_801D8234:
    lwz r0, 0x84(r30)
    cmpwi r0, 0x1
    beq @L_801D8248
    bge @L_801D8280
    b @L_801D8280
@L_801D8248:
    lwz r0, 0x14(r30)
    lwz r3, 0xb0(r30)
    rlwinm r0, r0, 2, 0, 29
    lwz r5, 0x70(r30)
    add r4, r30, r0
    lwz r0, 0x0(r3)
    lwz r4, 0x28(r4)
    rlwinm r3, r0, 1, 31, 31
    add r5, r5, r4
    srawi r4, r5, 31
    subfc r0, r0, r5
    adde r0, r4, r3
    rlwinm r31, r0, 0, 24, 31
    b @L_801D8288
@L_801D8280:
    bl fn_800EC954
    mr r31, r3
@L_801D8288:
    lwz r0, 0x88(r30)
    lwz r3, 0x90(r30)
    cmpw r0, r3
    beq @L_801D8310
    cmpwi r3, 0x0
    blt @L_801D8310
    lwz r0, 0x8c(r30)
    cmpwi r0, 0x1
    beq @L_801D82B4
    bge @L_801D82EC
    b @L_801D82EC
@L_801D82B4:
    lwz r0, 0x14(r30)
    lwz r3, 0xb0(r30)
    rlwinm r0, r0, 2, 0, 29
    lwz r5, 0x70(r30)
    add r4, r30, r0
    lwz r0, 0x0(r3)
    lwz r4, 0x28(r4)
    rlwinm r3, r0, 1, 31, 31
    add r5, r5, r4
    srawi r4, r5, 31
    subfc r0, r0, r5
    adde r0, r4, r3
    rlwinm r3, r0, 0, 24, 31
    b @L_801D82F4
@L_801D82EC:
    lwz r3, 0xa4(r30)
    bl fn_800EC1C8
@L_801D82F4:
    rlwinm. r0, r31, 0, 24, 31
    li r4, 0x0
    beq @L_801D830C
    rlwinm. r0, r3, 0, 24, 31
    beq @L_801D830C
    li r4, 0x1
@L_801D830C:
    rlwinm r31, r4, 0, 24, 31
@L_801D8310:
    rlwinm. r0, r31, 0, 24, 31
    beq @L_801D84D8
    lwz r0, 0x9c(r30)
    rlwinm. r0, r0, 0, 30, 30
    beq @L_801D8334
    li r0, 0x3
    li r3, 0x1
    stw r0, 0x6c(r30)
    b @L_801D84DC
@L_801D8334:
    li r3, 0x0
    b @L_801D84DC
    lwz r0, 0x7c(r30)
    rlwinm r0, r0, 0, 31, 31
    cmpwi r0, 0x1
    bne @L_801D8368
    lwz r3, -21528(r13)
    cmplwi r3, 0x0
    beq @L_801D8360
    bl fn_800F04BC
    b @L_801D84DC
@L_801D8360:
    li r3, 0x0
    b @L_801D84DC
@L_801D8368:
    lwz r3, 0x78(r30)
    bl fn_801666BC
    subi r3, r3, 0x2
    li r4, 0x1
    subfic r0, r3, 0x1
    orc r3, r4, r3
    rlwinm r0, r0, 31, 1, 31
    subf r0, r0, r3
    rlwinm. r0, r0, 1, 31, 31
    bne @L_801D8394
    li r4, 0x0
@L_801D8394:
    rlwinm r3, r4, 0, 24, 31
    b @L_801D84DC
    lwz r3, 0x78(r30)
    cmpwi r3, 0x0
    beq @L_801D84D8
    mr r31, r3
    bl fn_801310A8
    rlwinm. r0, r3, 0, 24, 31
    beq @L_801D83EC
    lwz r0, 0x88(r30)
    cmpwi r0, 0x0
    beq @L_801D83C8
    b @L_801D83E4
@L_801D83C8:
    mr r3, r31
    bl fn_8013151C
    bl fn_8013AB34
    rlwinm. r0, r3, 0, 24, 31
    beq @L_801D83E4
    li r0, 0x3
    stw r0, 0x6c(r30)
@L_801D83E4:
    li r3, 0x1
    b @L_801D84DC
@L_801D83EC:
    li r3, 0x0
    b @L_801D84DC
    lwz r3, 0x8c(r30)
    cmplwi r3, 0x0
    beq @L_801D84D8
    bl fn_80118DA8
    cmplwi r3, 0x0
    bne @L_801D841C
    lwz r3, 0x8c(r30)
    bl fn_80118D84
    cmplwi r3, 0x0
    beq @L_801D8424
@L_801D841C:
    li r3, 0x1
    b @L_801D84DC
@L_801D8424:
    li r3, 0x0
    b @L_801D84DC
    lwz r0, 0x78(r30)
    cmpwi r0, 0x0
    beq @L_801D8440
    blt @L_801D849C
    b @L_801D849C
@L_801D8440:
    lwz r4, 0x70(r30)
    lwz r3, 0x74(r30)
    lwz r0, 0x7c(r30)
    subf r3, r4, r3
    cmpw r3, r0
    blt @L_801D84D8
    lwz r4, 0xb0(r30)
    lwz r3, 0x24(r4)
    b @L_801D8484
@L_801D8464:
    lwz r0, 0x6c(r3)
    cmpwi r0, 0x3
    bne @L_801D8480
    li r0, -0x1
    li r3, 0x0
    stb r0, 0x15(r4)
    b @L_801D84DC
@L_801D8480:
    lwz r3, 0xa8(r3)
@L_801D8484:
    cmplwi r3, 0x0
    bne @L_801D8464
    li r0, 0x1
    li r3, 0x0
    stb r0, 0x15(r4)
    b @L_801D84DC
@L_801D849C:
    li r3, 0x0
    b @L_801D84DC
    li r3, 0x0
    b @L_801D84DC
@L_801D84AC:
    bl fn_800057A8
    cmpwi r3, 0x2
    bne @L_801D84C0
    li r3, 0x1
    bl fn_801D744C
@L_801D84C0:
    lis r3, -32728
    addi r3, r3, -27212
    crxor 6, 6, 6
    bl fn_800DD970
    li r3, 0x0
    b @L_801D84DC
@L_801D84D8:
    li r3, 0x1
@L_801D84DC:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    lwz r30, 0x8(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
