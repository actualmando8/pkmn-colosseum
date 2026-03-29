    nofralloc
    lwz r3, 0x60(r3)
    lis r5, lbl_80266DD8@ha
    addi r5, r5, lbl_80266DD8@l
    lwz r0, 0x1c(r3)
    slwi r0, r0, 4
    add r3, r5, r0
    lwz r0, 0x4(r3)
    cmpwi r0, 0x8
    beq @L_80026630
    li r0, 0x0
    stb r0, 0x67(r4)
    b @L_80026638
    @L_80026630:
    li r0, 0xff
    stb r0, 0x67(r4)
    @L_80026638:
    li r3, 0x0
    blr
