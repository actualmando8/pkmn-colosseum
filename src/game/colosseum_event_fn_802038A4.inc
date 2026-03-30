    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stmw r30, 0x8(r1)
    mr. r30, r3
    bne @L_802038C4
    li r0, 0x0
    b @L_802039E0
@L_802038C4:
    bl fn_801EF634
    rlwinm r0, r3, 0, 16, 31
    cmplwi r0, 0x1
    bne @L_802038DC
    li r0, 0x0
    b @L_802039E0
@L_802038DC:
    mr r3, r30
    li r4, 0x0
    li r5, 0xd6
    li r6, 0x0
    bl fn_8012640C
    mr. r31, r3
    bne @L_80203900
    li r0, 0x0
    b @L_802039E0
@L_80203900:
    bne @L_8020390C
    li r0, 0x0
    b @L_802039CC
@L_8020390C:
    bl fn_801EF634
    rlwinm r0, r3, 0, 16, 31
    cmplwi r0, 0x1
    bne @L_80203924
    li r0, 0x0
    b @L_802039CC
@L_80203924:
    mr r3, r31
    li r4, 0x0
    li r5, 0xcb
    li r6, 0x0
    bl fn_8012640C
    cmplwi r3, 0x0
    bne @L_80203948
    li r0, 0x0
    b @L_802039CC
@L_80203948:
    bl fn_80123FBC
    rlwinm. r0, r3, 0, 24, 31
    bne @L_8020395C
    li r0, 0x0
    b @L_802039CC
@L_8020395C:
    cmplwi r31, 0x0
    bne @L_8020396C
    li r3, 0x0
    b @L_80203980
@L_8020396C:
    mr r3, r31
    li r4, 0x0
    li r5, 0xcc
    li r6, 0x0
    bl fn_8012640C
@L_80203980:
    cmplwi r3, 0x0
    bne @L_80203990
    li r0, 0x0
    b @L_802039CC
@L_80203990:
    bl fn_80123FBC
    rlwinm. r0, r3, 0, 24, 31
    bne @L_802039A4
    li r0, 0x0
    b @L_802039CC
@L_802039A4:
    mr r3, r31
    li r4, 0x0
    li r5, 0xce
    li r6, 0x0
    bl fn_8012640C
    cmpwi r3, 0x0
    bge @L_802039C8
    li r0, 0x0
    b @L_802039CC
@L_802039C8:
    li r0, 0x1
@L_802039CC:
    rlwinm. r0, r0, 0, 24, 31
    bne @L_802039DC
    li r0, 0x0
    b @L_802039E0
@L_802039DC:
    li r0, 0x1
@L_802039E0:
    rlwinm. r0, r0, 0, 24, 31
    bne @L_802039F0
    li r3, 0x1
    b @L_80203A58
@L_802039F0:
    cmplwi r30, 0x0
    bne @L_80203A00
    li r3, 0x0
    b @L_80203A34
@L_80203A00:
    mr r3, r30
    li r4, 0x0
    li r5, 0xd6
    li r6, 0x0
    bl fn_8012640C
    cmplwi r3, 0x0
    bne @L_80203A24
    li r3, 0x0
    b @L_80203A34
@L_80203A24:
    li r4, 0x0
    li r5, 0xcc
    li r6, 0x0
    bl fn_8012640C
@L_80203A34:
    cmplwi r3, 0x0
    bne @L_80203A44
    li r3, 0x1
    b @L_80203A58
@L_80203A44:
    li r4, 0x0
    li r5, 0x7b
    li r6, 0x0
    bl fn_8012640C
    rlwinm r3, r3, 0, 24, 31
@L_80203A58:
    lmw r30, 0x8(r1)
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
