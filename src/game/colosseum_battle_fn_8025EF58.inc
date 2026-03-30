    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    stw r0, 0x34(r1)
    stw r31, 0x2c(r1)
    stw r30, 0x28(r1)
    stw r29, 0x24(r1)
    stw r28, 0x20(r1)
    lwz r0, -20920(r13)
    cmpwi r0, -0x1
    beq @L_8025F28C
    li r29, 0x0
    li r3, 0x0
    li r4, 0x0
    bl fn_80129280
    li r3, 0x0
    li r4, 0x2
    bl fn_80129280
    li r28, 0x0
    mr r31, r3
    b @L_8025EFF4
@L_8025EFA8:
    mr r3, r31
    clrlwi r4, r28, 16
    bl fn_8012AC08
    mr r30, r3
    bl fn_80123FBC
    clrlwi. r0, r3, 24
    beq @L_8025EFF0
    mr r3, r30
    bl fn_8011FC74
    clrlwi r0, r3, 24
    cmpwi r0, 0x1
    bne @L_8025EFF0
    mr r3, r30
    bl fn_8011EE40
    bl fn_801EEC74
    clrlwi. r0, r3, 24
    bne @L_8025EFF0
    addi r29, r29, 0x1
@L_8025EFF0:
    addi r28, r28, 0x1
@L_8025EFF4:
    clrlwi r0, r28, 16
    cmplwi r0, 0x6
    blt @L_8025EFA8
    bl fn_801CAF0C
    clrlwi r31, r3, 16
    li r3, 0x0
    li r4, 0x0
    bl fn_80129280
    li r3, 0x0
    li r4, 0x2
    bl fn_80129280
    li r3, 0x0
    li r4, 0x219
    bl fn_80129B2C
    clrlwi r0, r3, 24
    cmpwi r0, 0x1
    bne @L_8025F060
    clrlwi. r0, r29, 16
    beq @L_8025F058
    cmplwi r31, 0x0
    beq @L_8025F050
    li r0, 0x2
    b @L_8025F074
@L_8025F050:
    li r0, 0x1
    b @L_8025F074
@L_8025F058:
    li r0, 0x3
    b @L_8025F074
@L_8025F060:
    cmplwi r31, 0x0
    beq @L_8025F070
    li r0, 0x4
    b @L_8025F074
@L_8025F070:
    li r0, 0x5
@L_8025F074:
    lwz r3, -20920(r13)
    mr r4, r0
    cmpwi r3, 0x0
    bne @L_8025F0B8
    clrlwi r0, r0, 16
    cmplwi r0, 0x1
    bne @L_8025F098
    li r4, 0x5
    b @L_8025F0C4
@L_8025F098:
    cmplwi r0, 0x2
    bne @L_8025F0A8
    li r4, 0x4
    b @L_8025F0C4
@L_8025F0A8:
    cmplwi r0, 0x3
    bne @L_8025F0C4
    li r4, 0x5
    b @L_8025F0C4
@L_8025F0B8:
    cmpwi r3, 0x1
    bne @L_8025F0C4
    li r4, 0x2
@L_8025F0C4:
    clrlwi r0, r4, 16
    cmplwi r0, 0x2
    bne @L_8025F174
    addi r3, r1, 0x8
    bl fn_8025DD14
    li r3, 0x3c8
    li r4, 0x0
    li r5, 0xff
    bl fn_80165668
    addi r28, r13, -20920
    li r3, 0x0
    lwz r0, 0x4(r28)
    li r4, 0x3
    clrlwi r5, r0, 16
    bl fn_8012A5B0
    li r4, 0x0
    li r5, 0x6e
    li r6, 0x0
    bl fn_8012640C
    li r5, 0x1
    lis r4, 0x8048
    stwu r5, -32068(r4)
    lwz r0, 0x4(r28)
    sth r3, 0x4(r4)
    li r3, 0x3
    lfs f1, -20512(r2)
    stw r0, 0x8(r4)
    bl fn_801C41C8
    li r3, 0x1
    bl fn_801C40F0
    li r3, 0x385
    bl fn_800FF730
    li r3, 0x0
    li r4, 0x0
    bl fn_8011288C
    bl fn_800F0308
    lfs f1, -20500(r2)
    li r3, 0x2
    bl fn_801C41C8
    li r3, 0x1
    bl fn_801C40F0
    addi r3, r1, 0x8
    bl fn_8025DCBC
    b @L_8025F28C
@L_8025F174:
    cmplwi r0, 0x4
    bne @L_8025F27C
    lwz r0, -20924(r13)
    cmpwi r0, 0x1
    bne @L_8025F1A0
    lwz r3, -20928(r13)
    li r4, 0x1
    bl fn_80118A68
    li r0, 0x0
    stw r0, -20928(r13)
    stw r0, -20924(r13)
@L_8025F1A0:
    bl fn_801CAF0C
    clrlwi. r0, r3, 16
    bne @L_8025F1C0
    li r0, -0x1
    addi r3, r13, -20920
    stw r0, -20920(r13)
    stw r0, 0x4(r3)
    b @L_8025F28C
@L_8025F1C0:
    li r3, 0x3b0f
    li r4, 0x1
    li r5, 0x0
    bl fn_801067E8
    li r3, 0x1
    bl fn_801065B8
    li r3, 0x7
    li r4, 0x0
    li r5, 0x0
    bl fn_8001BDF4
    mr r31, r3
    cmpwi r31, -0x1
    beq @L_8025F28C
    clrlwi r5, r31, 16
    li r3, 0x0
    li r4, 0x3
    bl fn_8012A5B0
    li r4, 0x0
    li r5, 0x6e
    li r6, 0x0
    bl fn_8012640C
    li r0, 0x0
    lis r4, 0x8048
    stwu r0, -32068(r4)
    lfs f1, -20512(r2)
    sth r3, 0x4(r4)
    li r3, 0x3
    stw r31, 0x8(r4)
    bl fn_801C41C8
    li r3, 0x1
    bl fn_801C40F0
    addi r3, r1, 0x8
    bl fn_8025DD14
    li r3, 0x385
    bl fn_800FF730
    li r3, 0x0
    li r4, 0x0
    bl fn_8011288C
    bl fn_800F0308
    lfs f1, -20500(r2)
    li r3, 0x2
    bl fn_801C41C8
    li r3, 0x1
    bl fn_801C40F0
    addi r3, r1, 0x8
    bl fn_8025DCBC
    b @L_8025F28C
@L_8025F27C:
    li r0, -0x1
    addi r3, r13, -20920
    stw r0, -20920(r13)
    stw r0, 0x4(r3)
@L_8025F28C:
    lwz r0, 0x34(r1)
    lwz r31, 0x2c(r1)
    lwz r30, 0x28(r1)
    lwz r29, 0x24(r1)
    lwz r28, 0x20(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr
