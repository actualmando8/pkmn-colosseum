    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    mr. r31, r3
    stw r30, 0x8(r1)
    bne @L_801DD480
    li r3, 0x0
    b @L_801DD5D0
@L_801DD480:
    bl fn_801DD5E8
    rlwinm. r0, r3, 0, 24, 31
    beq @L_801DD5BC
    lwz r3, 0x0(r31)
    lwz r4, 0x4(r31)
    bl fn_800F9318
    mr r0, r3
    mr r3, r31
    stw r0, 0x24(r31)
    mr r30, r0
    bl fn_801DF160
    mr r4, r3
    mr r3, r30
    bl fn_800EB268
    lwz r4, 0xc(r31)
    cmplwi r4, 0x0
    beq @L_801DD4E0
    lwz r3, 0x0(r31)
    bl fn_800F9318
    bl fn_801195AC
    stw r3, 0x28(r31)
    mr r3, r30
    lwz r4, 0x28(r31)
    bl fn_800E3C08
@L_801DD4E0:
    mr r3, r30
    li r4, 0x1
    bl fn_800EC188
    lis r4, -32738
    mr r3, r30
    addi r4, r4, -6572
    mr r5, r31
    bl fn_800EC154
    mr r3, r31
    li r4, 0x1
    li r5, 0x1
    bl fn_801DEF0C
    mr r3, r31
    li r4, 0x0
    bl fn_801DA4E8
    lis r4, -32713
    mr r3, r30
    addi r4, r4, 10160
    bl fn_800E43A4
    lis r4, -32713
    mr r3, r30
    addi r4, r4, 10160
    bl fn_800E4170
    lis r4, -32713
    mr r3, r30
    addi r4, r4, 10172
    bl fn_800E407C
    bl fn_801DB848
    cmplwi r3, 0x0
    beq @L_801DD5B4
    bl fn_801DB850
    cmpwi r3, 0x0
    beq @L_801DD5B4
    mr r3, r30
    li r4, 0x1
    bl fn_800E9108
    bl fn_801DB848
    mr r4, r3
    mr r3, r30
    bl fn_800E8FE8
    bl fn_801DB858
    mr r31, r3
    bl fn_801DB850
    mr r4, r3
    mr r3, r30
    mr r5, r31
    bl fn_800E900C
    mr r3, r30
    li r4, 0x1
    bl fn_800E3C94
    mr r3, r30
    li r4, 0x1
    bl fn_800E3B44
@L_801DD5B4:
    li r3, 0x1
    b @L_801DD5D0
@L_801DD5BC:
    lis r3, -32728
    addi r3, r3, -26216
    crxor 6, 6, 6
    bl fn_800DD970
    li r3, 0x0
@L_801DD5D0:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    lwz r30, 0x8(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
