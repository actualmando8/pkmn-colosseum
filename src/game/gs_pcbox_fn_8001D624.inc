    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    stw r30, 0x8(r1)
    mr r30, r3
    mr r31, r4
    li r4, 0x0
    li r5, 0x7b
    li r6, 0x0
    bl fn_8012640C
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_8001D664
    li r3, 0x1
    b @L_8001D6C8
    @L_8001D664:
    mr r3, r30
    bl fn_80120FE0
    clrlwi r0, r3, 16
    cmpwi r0, 0x3c
    beq @L_8001D6AC
    bge @L_8001D68C
    cmpwi r0, 0x3a
    beq @L_8001D69C
    bge @L_8001D6A4
    b @L_8001D6C4
    @L_8001D68C:
    cmpwi r0, 0x3e
    beq @L_8001D6BC
    bge @L_8001D6C4
    b @L_8001D6B4
    @L_8001D69C:
    li r3, 0x2
    b @L_8001D6C8
    @L_8001D6A4:
    li r3, 0x3
    b @L_8001D6C8
    @L_8001D6AC:
    li r3, 0x4
    b @L_8001D6C8
    @L_8001D6B4:
    li r3, 0x5
    b @L_8001D6C8
    @L_8001D6BC:
    li r3, 0x6
    b @L_8001D6C8
    @L_8001D6C4:
    li r3, 0x0
    @L_8001D6C8:
    clrlwi r0, r31, 24
    cmplwi r0, 0x0
    bne @L_8001D6EC
    clrlwi r0, r3, 16
    lis r3, lbl_802E4EB8@ha
    slwi r0, r0, 1
    addi r3, r3, lbl_802E4EB8@l
    lhzx r3, r3, r0
    b @L_8001D700
    @L_8001D6EC:
    clrlwi r0, r3, 16
    lis r3, lbl_802E4EC8@ha
    slwi r0, r0, 1
    addi r3, r3, lbl_802E4EC8@l
    lhzx r3, r3, r0
    @L_8001D700:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    lwz r30, 0x8(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
