    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    li r5, 0xee
    li r6, 0x0
    stw r0, 0x14(r1)
    stmw r30, 0x8(r1)
    mr r30, r4
    li r4, 0x0
    bl fn_8012640C
    rlwinm. r0, r30, 0, 16, 31
    mr r31, r3
    bne @L_802029E8
    cmplwi r31, 0x0
    beq @L_80202A18
    li r4, 0x1
    bl fn_801DA36C
    mr r3, r31
    li r4, 0x2
    bl fn_801DA36C
    b @L_80202A18
@L_802029E8:
    cmplwi r31, 0x0
    beq @L_80202A18
    cmplwi r0, 0x8
    bne @L_80202A00
    li r4, 0x1
    bl fn_801DA36C
@L_80202A00:
    rlwinm r0, r30, 0, 16, 31
    cmplwi r0, 0x7
    bne @L_80202A18
    mr r3, r31
    li r4, 0x2
    bl fn_801DA36C
@L_80202A18:
    lmw r30, 0x8(r1)
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
