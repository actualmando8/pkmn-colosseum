    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    li r5, 0x0
    stw r0, 0x24(r1)
    li r0, 0x0
    stw r31, 0x1c(r1)
    stw r30, 0x18(r1)
    mr r30, r3
    stw r29, 0x14(r1)
    li r29, 0x0
    b @L_801BFF68
@L_801BFF44:
    mulli r4, r5, 0x60
    lis r3, lbl_80466BC0@ha
    addi r3, r3, lbl_80466BC0@l
    add r3, r3, r4
    lwz r3, 0x5c(r3)
    cmpwi r3, 0x6
    bne @L_801BFF64
    b @L_801BFF74
@L_801BFF64:
    addi r5, r5, 0x1
@L_801BFF68:
    cmpwi r5, 0x3
    blt @L_801BFF44
    li r5, -0x1
@L_801BFF74:
    mr r31, r5
    cmpwi r31, -0x1
    beq @L_801BFFF0
    mulli r4, r31, 0x60
    lis r3, lbl_80466BC0@ha
    addi r0, r3, lbl_80466BC0@l
    add r3, r0, r4
    lwz r3, 0x58(r3)
    bl fn_800AA198
    mulli r4, r31, 0x60
    lis r3, lbl_80466BC0@ha
    addi r0, r3, lbl_80466BC0@l
    add r3, r0, r4
    lbz r0, 0xb4(r3)
    cmplwi r0, 0x0
    beq @L_801BFFE4
    mulli r4, r31, 0x60
    lis r3, lbl_80466BC0@ha
    addi r0, r3, lbl_80466BC0@l
    add r3, r0, r4
    addi r3, r3, 0x60
    bl fn_800A94AC
    mulli r4, r31, 0x60
    lis r3, lbl_80466BC0@ha
    addi r0, r3, lbl_80466BC0@l
    add r3, r0, r4
    lwz r3, 0x9c(r3)
    bl fn_800AA204
@L_801BFFE4:
    li r0, 0x1
    li r29, 0x1
    b @L_801C0124
@L_801BFFF0:
    lis r3, lbl_80466BC0@ha
    addi r3, r3, lbl_80466BC0@l
    lwz r3, 0x1d4(r3)
    cmpwi r3, 0x1
    bne @L_801C0124
    lis r3, lbl_80466BC0@ha
    addi r3, r3, lbl_80466BC0@l
    lwz r3, 0x178(r3)
    cmpwi r3, 0x1
    bne @L_801C0124
    li r5, 0x0
    b @L_801C0044
@L_801C0020:
    mulli r4, r5, 0x60
    lis r3, lbl_80466BC0@ha
    addi r3, r3, lbl_80466BC0@l
    add r3, r3, r4
    lwz r3, 0x5c(r3)
    cmpwi r3, 0x7
    bne @L_801C0040
    b @L_801C0050
@L_801C0040:
    addi r5, r5, 0x1
@L_801C0044:
    cmpwi r5, 0x3
    blt @L_801C0020
    li r5, -0x1
@L_801C0050:
    mr r31, r5
    cmpwi r31, -0x1
    bne @L_801C00D0
    li r5, 0x0
    b @L_801C0088
@L_801C0064:
    mulli r4, r5, 0x60
    lis r3, lbl_80466BC0@ha
    addi r0, r3, lbl_80466BC0@l
    add r3, r0, r4
    lwz r0, 0x5c(r3)
    cmpwi r0, 0x2
    bne @L_801C0084
    b @L_801C0094
@L_801C0084:
    addi r5, r5, 0x1
@L_801C0088:
    cmpwi r5, 0x3
    blt @L_801C0064
    li r5, -0x1
@L_801C0094:
    mr r31, r5
    cmpwi r31, -0x1
    bne @L_801C00B4
    lis r4, lbl_802756F8@ha
    addi r3, r2, -22384
    addi r5, r4, lbl_802756F8@l
    li r4, 0xfc
    bl fn_80196E10
@L_801C00B4:
    mulli r4, r31, 0x60
    lis r3, lbl_80466BC0@ha
    addi r0, r3, lbl_80466BC0@l
    add r3, r0, r4
    lwz r3, 0x58(r3)
    bl fn_800AA198
    li r0, 0x1
@L_801C00D0:
    mulli r5, r31, 0x60
    lis r4, lbl_80466BC0@ha
    lis r3, lbl_80466BC0@ha
    addi r4, r4, lbl_80466BC0@l
    add r4, r4, r5
    li r5, 0x8
    stw r5, 0x5c(r4)
    addi r3, r3, lbl_80466BC0@l
    lbz r3, 0x1d0(r3)
    cmplwi r3, 0x0
    beq @L_801C0120
    lis r3, lbl_80466BC0@ha
    addi r3, r3, lbl_80466BC0@l
    addi r3, r3, 0x17c
    bl fn_800A94AC
    lis r3, lbl_80466BC0@ha
    addi r3, r3, lbl_80466BC0@l
    lwz r3, 0x1b8(r3)
    bl fn_800AA204
    li r0, 0x1
@L_801C0120:
    li r29, 0x1
@L_801C0124:
    cmpwi r0, 0x0
    beq @L_801C0130
    bl fn_800AA068
@L_801C0130:
    cmpwi r29, 0x0
    beq @L_801C0144
    lwz r3, lbl_8047B384(r13)
    addi r0, r3, 0x1
    stw r0, lbl_8047B384(r13)
@L_801C0144:
    lis r3, lbl_80466BC0@ha
    lwz r4, lbl_8047B380(r13)
    addi r3, r3, lbl_80466BC0@l
    lwz r0, 0x1ec(r3)
    addi r3, r4, 0x1
    stw r3, lbl_8047B380(r13)
    cmpw r3, r0
    blt @L_801C0180
    lis r3, lbl_80466BC0@ha
    lwz r4, lbl_8047B384(r13)
    li r0, 0x0
    addi r3, r3, lbl_80466BC0@l
    stw r0, lbl_8047B384(r13)
    stw r4, 0x1f0(r3)
    stw r0, lbl_8047B380(r13)
@L_801C0180:
    lis r3, lbl_80466BC0@ha
    addi r3, r3, lbl_80466BC0@l
    lwz r0, 0x1d8(r3)
    cmplwi r0, 0x0
    beq @L_801C01AC
    lis r4, lbl_80466BC0@ha
    mr r3, r30
    addi r4, r4, lbl_80466BC0@l
    lwz r12, 0x1d8(r4)
    mtctr r12
    bctrl
@L_801C01AC:
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr

