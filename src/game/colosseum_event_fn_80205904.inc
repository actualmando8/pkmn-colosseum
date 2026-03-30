    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stmw r29, 0x14(r1)
    mr. r29, r3
    mr r30, r4
    bne @L_80205928
    li r3, 0x0
    b @L_80205A68
@L_80205928:
    cmplwi r30, 0x0
    bne @L_80205938
    li r0, 0x0
    b @L_802059F8
@L_80205938:
    bl fn_801EF634
    rlwinm r0, r3, 0, 16, 31
    cmplwi r0, 0x1
    bne @L_80205950
    li r0, 0x0
    b @L_802059F8
@L_80205950:
    mr r3, r30
    li r4, 0x0
    li r5, 0xcb
    li r6, 0x0
    bl fn_8012640C
    cmplwi r3, 0x0
    bne @L_80205974
    li r0, 0x0
    b @L_802059F8
@L_80205974:
    bl fn_80123FBC
    rlwinm. r0, r3, 0, 24, 31
    bne @L_80205988
    li r0, 0x0
    b @L_802059F8
@L_80205988:
    cmplwi r30, 0x0
    bne @L_80205998
    li r3, 0x0
    b @L_802059AC
@L_80205998:
    mr r3, r30
    li r4, 0x0
    li r5, 0xcc
    li r6, 0x0
    bl fn_8012640C
@L_802059AC:
    cmplwi r3, 0x0
    bne @L_802059BC
    li r0, 0x0
    b @L_802059F8
@L_802059BC:
    bl fn_80123FBC
    rlwinm. r0, r3, 0, 24, 31
    bne @L_802059D0
    li r0, 0x0
    b @L_802059F8
@L_802059D0:
    mr r3, r30
    li r4, 0x0
    li r5, 0xce
    li r6, 0x0
    bl fn_8012640C
    cmpwi r3, 0x0
    bge @L_802059F4
    li r0, 0x0
    b @L_802059F8
@L_802059F4:
    li r0, 0x1
@L_802059F8:
    rlwinm. r0, r0, 0, 24, 31
    bne @L_80205A08
    li r3, 0x0
    b @L_80205A68
@L_80205A08:
    mr r3, r30
    li r4, 0x0
    li r5, 0xce
    li r6, 0x0
    bl fn_8012640C
    extsh r31, r3
    li r30, 0x0
    b @L_80205A58
@L_80205A28:
    mr r3, r29
    rlwinm r6, r30, 0, 24, 31
    li r4, 0x0
    li r5, 0xfd
    bl fn_8012640C
    extsh. r0, r3
    blt @L_80205A54
    cmpw r0, r31
    bne @L_80205A54
    li r3, 0x1
    b @L_80205A68
@L_80205A54:
    addi r30, r30, 0x1
@L_80205A58:
    rlwinm r0, r30, 0, 24, 31
    cmplwi r0, 0xc
    blt @L_80205A28
    li r3, 0x0
@L_80205A68:
    lmw r29, 0x14(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
