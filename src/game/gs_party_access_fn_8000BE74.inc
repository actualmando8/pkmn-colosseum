    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    mr r31, r3
    li r0, 0x0
    li r3, 0x1
    stw r0, 0x8(r1)
    bl fn_800F7BC4
    rlwinm r0, r3, 0, 26, 26
    cmplwi r0, 0x0
    beq @L_8000BEC4
    addi r4, r1, 0x8
    li r3, 0x0
    bl fn_8001E3E0
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    bne @L_8000BEC4
    li r3, -0x1
    b @L_8000BF8C
    @L_8000BEC4:
    lis r3, lbl_802E28F0@ha
    li r4, 0x0
    addi r3, r3, lbl_802E28F0@l
    li r0, 0x99
    mtctr r0
    @L_8000BED8:
    lwz r0, 0x0(r3)
    cmpw r31, r0
    bne @L_8000BF7C
    cmpwi r31, 0x66
    beq @L_8000BF3C
    bge @L_8000BF08
    cmpwi r31, 0x64
    beq @L_8000BF24
    bge @L_8000BF30
    cmpwi r31, 0x63
    bge @L_8000BF18
    b @L_8000BF5C
    @L_8000BF08:
    cmpwi r31, 0x68
    beq @L_8000BF54
    bge @L_8000BF5C
    b @L_8000BF48
    @L_8000BF18:
    li r0, 0xb
    stw r0, 0x8(r1)
    b @L_8000BF5C
    @L_8000BF24:
    li r0, 0xa
    stw r0, 0x8(r1)
    b @L_8000BF5C
    @L_8000BF30:
    li r0, 0x8
    stw r0, 0x8(r1)
    b @L_8000BF5C
    @L_8000BF3C:
    li r0, 0xd
    stw r0, 0x8(r1)
    b @L_8000BF5C
    @L_8000BF48:
    li r0, 0x0
    stw r0, 0x8(r1)
    b @L_8000BF5C
    @L_8000BF54:
    li r0, 0x0
    stw r0, 0x8(r1)
    @L_8000BF5C:
    lis r3, lbl_802E28F0@ha
    slwi r0, r4, 3
    addi r3, r3, lbl_802E28F0@l
    lwz r4, 0x8(r1)
    add r3, r3, r0
    lwz r3, 0x4(r3)
    bl fn_80113828
    b @L_8000BF88
    @L_8000BF7C:
    addi r3, r3, 0x8
    addi r4, r4, 0x1
    bdnz @L_8000BED8
    @L_8000BF88:
    li r3, 0x0
    @L_8000BF8C:
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
