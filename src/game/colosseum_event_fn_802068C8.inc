    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stmw r28, 0x10(r1)
    mr. r28, r3
    mr r30, r4
    mr r29, r5
    beq @L_802069F0
    cmplwi r30, 0x0
    beq @L_802069F0
    bne @L_802068FC
    li r31, 0x0
    b @L_80206914
@L_802068FC:
    mr r3, r30
    li r4, 0x0
    li r5, 0xcc
    li r6, 0x0
    bl fn_8012640C
    mr r31, r3
@L_80206914:
    mr r3, r28
    bl fn_80206C94
    mr r3, r28
    mr r7, r30
    li r4, 0x0
    li r5, 0xd5
    li r6, 0x0
    bl fn_801254B4
    mr r3, r28
    mr r7, r30
    li r4, 0x0
    li r5, 0xd6
    li r6, 0x0
    bl fn_801254B4
    cmplwi r29, 0x0
    beq @L_80206970
    mr r3, r28
    mr r7, r29
    li r4, 0x0
    li r5, 0xee
    li r6, 0x0
    bl fn_801254B4
    bl fn_801F198C
@L_80206970:
    mr r3, r31
    li r4, 0x0
    li r5, 0x6e
    li r6, 0x0
    bl fn_8012640C
    rlwinm r30, r3, 0, 16, 31
    li r29, 0x0
    b @L_802069C0
@L_80206990:
    mr r4, r30
    mr r6, r29
    li r3, 0x0
    li r5, 0x16
    bl fn_8012640C
    rlwinm r7, r3, 0, 16, 31
    rlwinm r6, r29, 0, 24, 31
    mr r3, r28
    li r4, 0x0
    li r5, 0xff
    bl fn_801254B4
    addi r29, r29, 0x1
@L_802069C0:
    rlwinm r0, r29, 0, 16, 31
    cmplwi r0, 0x2
    blt @L_80206990
    mr r3, r31
    bl fn_801248C4
    mr r0, r3
    mr r3, r28
    rlwinm r7, r0, 0, 16, 31
    li r4, 0x0
    li r5, 0x100
    li r6, 0x0
    bl fn_801254B4
@L_802069F0:
    lmw r28, 0x10(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
