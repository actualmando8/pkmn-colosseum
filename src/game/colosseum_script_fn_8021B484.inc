    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    li r3, 0x11
    li r4, 0x0
    stw r0, 0x34(r1)
    stmw r28, 0x20(r1)
    bl fn_801F025C
    mr r0, r3
    li r3, 0x12
    mr r28, r0
    li r4, 0x0
    bl fn_801F025C
    mr r0, r3
    li r3, 0x0
    mr r30, r0
    mr r4, r30
    bl fn_801F4354
    addi r4, r1, 0x8
    bl fn_801F87CC
    rlwinm. r0, r3, 0, 16, 31
    mr r31, r3
    bne @L_8021B4EC
@L_8021B4DC:
    lwz r3, lbl_8047B610(r13)
    lwz r0, 0x1(r3)
    stw r0, lbl_8047B610(r13)
    b @L_8021B5FC
@L_8021B4EC:
    mr r3, r28
    bl fn_80203E0C
    mr r28, r3
    mr r3, r30
    bl fn_80203E0C
    mr r29, r3
    rlwinm r3, r28, 0, 24, 31
    rlwinm r0, r29, 0, 24, 31
    cmplw r3, r0
    blt @L_8021B528
@L_8021B514:
    lis r3, lbl_80378CCE@ha
    li r4, 0x1
    addi r0, r3, lbl_80378CCE@l
    stw r0, lbl_8047B610(r13)
    b @L_8021B568
@L_8021B528:
    bl fn_800E0C54
    rlwinm r5, r28, 0, 24, 31
    rlwinm r4, r29, 0, 24, 31
    rlwinm r6, r3, 0, 24, 31
    rlwinm r0, r29, 30, 26, 31
    add r3, r5, r4
    mullw r3, r3, r6
    srawi r3, r3, 8
    addi r3, r3, 0x1
    rlwinm r3, r3, 0, 16, 31
    cmpw r3, r0
    bgt @L_8021B514
    lwz r3, lbl_8047B610(r13)
    li r4, 0x0
    lwz r0, 0x1(r3)
    stw r0, lbl_8047B610(r13)
@L_8021B568:
    rlwinm. r0, r4, 0, 24, 31
    beq @L_8021B5FC
    bl fn_800E0C54
    rlwinm r5, r3, 0, 16, 31
    rlwinm r4, r31, 0, 16, 31
    divw r0, r5, r4
    addi r3, r1, 0x8
    mullw r0, r0, r4
    subf r0, r0, r5
    clrlslwi r0, r0, 16, 2
    lwzx r3, r3, r0
    cmplwi r3, 0x0
    beq @L_8021B4DC
    li r4, 0x0
    li r5, 0xce
    li r6, 0x0
    bl fn_8012640C
    extsh. r28, r3
    blt @L_8021B4DC
    mr r7, r30
    li r3, 0x0
    li r4, 0x0
    li r5, 0x45
    li r6, 0x0
    bl fn_801F4C14
    mr r3, r30
    mr r7, r28
    li r4, 0x0
    li r5, 0x121
    li r6, 0x0
    bl fn_801254B4
    mr r3, r30
    li r4, 0x0
    li r5, 0x119
    li r6, 0x0
    li r7, 0x1
    bl fn_801254B4
@L_8021B5FC:
    lmw r28, 0x20(r1)
    lwz r0, 0x34(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr
