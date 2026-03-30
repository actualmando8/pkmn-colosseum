    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stmw r29, 0x14(r1)
    mr r31, r3
    bl fn_802062FC
    rlwinm. r0, r3, 0, 24, 31
    bne @L_8022EC68
    li r3, 0x0
    b @L_8022EDD8
@L_8022EC68:
    mr r3, r31
    bl fn_80203D3C
    mr r30, r3
    mr r3, r31
    bl fn_80207BF4
    rlwinm r0, r30, 0, 16, 31
    cmplwi r0, 0x181
    bne @L_8022EC94
    rlwinm r0, r3, 0, 16, 31
    cmplwi r0, 0x3b
    beq @L_8022EC9C
@L_8022EC94:
    li r3, 0x0
    b @L_8022EDD8
@L_8022EC9C:
    li r3, 0x0
    li r4, 0x1
    bl fn_801F453C
    rlwinm. r29, r3, 0, 24, 31
    li r30, 0x0
    beq @L_8022ECBC
    cmplwi r29, 0x3
    bne @L_8022ECFC
@L_8022ECBC:
    mr r3, r31
    li r4, 0x0
    bl fn_80207AE0
    rlwinm. r0, r3, 0, 24, 31
    bne @L_8022ECFC
    li r30, 0x0
    b @L_8022ECEC
@L_8022ECD8:
    mr r3, r31
    mr r4, r30
    li r5, 0x0
    bl fn_80207B5C
    addi r30, r30, 0x1
@L_8022ECEC:
    rlwinm r0, r30, 0, 24, 31
    cmplwi r0, 0x2
    blt @L_8022ECD8
    li r30, 0x1
@L_8022ECFC:
    cmplwi r29, 0x1
    bne @L_8022ED44
    mr r3, r31
    li r4, 0xa
    bl fn_80207AE0
    rlwinm. r0, r3, 0, 24, 31
    bne @L_8022ED44
    li r30, 0x0
    b @L_8022ED34
@L_8022ED20:
    mr r3, r31
    mr r4, r30
    li r5, 0xa
    bl fn_80207B5C
    addi r30, r30, 0x1
@L_8022ED34:
    rlwinm r0, r30, 0, 24, 31
    cmplwi r0, 0x2
    blt @L_8022ED20
    li r30, 0x2
@L_8022ED44:
    cmplwi r29, 0x2
    bne @L_8022ED8C
    mr r3, r31
    li r4, 0xb
    bl fn_80207AE0
    rlwinm. r0, r3, 0, 24, 31
    bne @L_8022ED8C
    li r30, 0x0
    b @L_8022ED7C
@L_8022ED68:
    mr r3, r31
    mr r4, r30
    li r5, 0xb
    bl fn_80207B5C
    addi r30, r30, 0x1
@L_8022ED7C:
    rlwinm r0, r30, 0, 24, 31
    cmplwi r0, 0x2
    blt @L_8022ED68
    li r30, 0x3
@L_8022ED8C:
    cmplwi r29, 0x4
    bne @L_8022EDD4
    mr r3, r31
    li r4, 0xf
    bl fn_80207AE0
    rlwinm. r0, r3, 0, 24, 31
    bne @L_8022EDD4
    li r30, 0x0
    b @L_8022EDC4
@L_8022EDB0:
    mr r3, r31
    mr r4, r30
    li r5, 0xf
    bl fn_80207B5C
    addi r30, r30, 0x1
@L_8022EDC4:
    rlwinm r0, r30, 0, 24, 31
    cmplwi r0, 0x2
    blt @L_8022EDB0
    li r30, 0x4
@L_8022EDD4:
    mr r3, r30
@L_8022EDD8:
    lmw r29, 0x14(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
