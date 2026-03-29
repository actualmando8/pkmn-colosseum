    nofralloc
    stwu r1, -0x50(r1)
    mflr r0
    stw r0, 0x54(r1)
    addi r3, r1, 0x8
    stw r31, 0x4c(r1)
    stw r30, 0x48(r1)
    stw r29, 0x44(r1)
    stw r28, 0x40(r1)
    bl fn_800AB150
    lis r3, lbl_80401C10@ha
    addi r30, r1, 0x8
    addi r31, r3, lbl_80401C10@l
    li r29, 0x0
@L_800F829C:
    lwz r0, 0x0(r31)
    addi r4, r29, 0x1
    li r3, 0x0
    cmpw r0, r4
    bne @L_800F82B8
    mr r28, r31
    b @L_800F82F0
@L_800F82B8:
    addi r28, r31, 0x6c
    lwz r0, 0x6c(r31)
    cmpw r0, r4
    bne @L_800F82CC
    b @L_800F82F0
@L_800F82CC:
    lwzu r0, 0x6c(r28)
    cmpw r0, r4
    bne @L_800F82DC
    b @L_800F82F0
@L_800F82DC:
    lwzu r0, 0x6c(r28)
    cmpw r0, r4
    bne @L_800F82EC
    b @L_800F82F0
@L_800F82EC:
    mr r28, r3
@L_800F82F0:
    cmplwi r28, 0x0
    beq @L_800F83E8
    lbz r0, 0xa(r30)
    extsb r0, r0
    cmpwi r0, -0x1
    beq @L_800F8398
    bge @L_800F8310
    b @L_800F83E8
@L_800F8310:
    cmpwi r0, 0x1
    bge @L_800F83E8
    lwz r0, 0xc(r28)
    cmpwi r0, 0x3
    bne @L_800F8358
    mr r3, r29
    bl fn_800D0F44
    lis r0, 0x900
    cmpw r3, r0
    beq @L_800F833C
    b @L_800F8348
@L_800F833C:
    li r0, 0x0
    stw r0, 0x4(r28)
    b @L_800F8350
@L_800F8348:
    li r0, 0x2
    stw r0, 0x4(r28)
@L_800F8350:
    li r0, 0x0
    stw r0, 0xc(r28)
@L_800F8358:
    lbz r0, 0x3(r30)
    mr r4, r30
    addi r3, r28, 0x18
    li r5, 0xc
    neg r0, r0
    stb r0, 0x3(r30)
    lbz r0, 0x5(r30)
    neg r0, r0
    stb r0, 0x5(r30)
    bl memcpy
    lis r0, 0x8000
    lwz r3, lbl_8047AC4C(r13)
    srw r0, r0, r29
    andc r0, r3, r0
    stw r0, lbl_8047AC4C(r13)
    b @L_800F83E8
@L_800F8398:
    mr r3, r29
    bl fn_800D0F44
    cmplwi r3, 0x8
    bne @L_800F83B4
    li r0, 0x3
    stw r0, 0xc(r28)
    b @L_800F83C4
@L_800F83B4:
    cmplwi r3, 0x40
    bne @L_800F83C4
    li r0, 0x4
    stw r0, 0xc(r28)
@L_800F83C4:
    addi r3, r28, 0x18
    li r4, 0x0
    li r5, 0xc
    bl memset
    lis r0, 0x8000
    lwz r3, lbl_8047AC4C(r13)
    srw r0, r0, r29
    or r0, r3, r0
    stw r0, lbl_8047AC4C(r13)
@L_800F83E8:
    addi r29, r29, 0x1
    addi r30, r30, 0xc
    cmpwi r29, 0x4
    blt @L_800F829C
    bl fn_800F8428
    lwz r3, lbl_8047AC48(r13)
    addi r0, r3, 0x1
    stw r0, lbl_8047AC48(r13)
    lwz r31, 0x4c(r1)
    lwz r30, 0x48(r1)
    lwz r29, 0x44(r1)
    lwz r28, 0x40(r1)
    lwz r0, 0x54(r1)
    mtlr r0
    addi r1, r1, 0x50
    blr
