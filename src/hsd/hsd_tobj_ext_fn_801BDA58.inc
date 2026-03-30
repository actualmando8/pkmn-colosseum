    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    mr r31, r3
    stw r30, 0x8(r1)
    b @L_801BDD54
@L_801BDA74:
    lwz r0, 0xc(r31)
    cmpwi r0, 0xff
    beq @L_801BDD50
    lwz r0, 0x4c(r31)
    rlwinm. r0, r0, 0, 7, 7
    beq @L_801BDC40
    lwz r0, 0x4c(r31)
    rlwinm r0, r0, 0, 28, 31
    cmpwi r0, 0x4
    beq @L_801BDB24
    bge @L_801BDAB4
    cmpwi r0, 0x3
    bge @L_801BDAC4
    cmpwi r0, 0x1
    bge @L_801BDAE4
    b @L_801BDB24
@L_801BDAB4:
    cmpwi r0, 0x6
    beq @L_801BDB04
    bge @L_801BDB24
    b @L_801BDAE4
@L_801BDAC4:
    lwz r3, 0xa4(r31)
    li r4, 0x0
    lwz r8, 0x14(r31)
    li r5, 0x0
    li r6, 0x0
    li r7, 0x0
    bl fn_800B857C
    b @L_801BDB6C
@L_801BDAE4:
    lwz r3, 0xa4(r31)
    li r4, 0x0
    lwz r8, 0x14(r31)
    li r5, 0x1
    li r6, 0x1e
    li r7, 0x1
    bl fn_800B857C
    b @L_801BDB6C
@L_801BDB04:
    lwz r3, 0xa4(r31)
    li r4, 0x0
    lwz r8, 0x14(r31)
    li r5, 0x0
    li r6, 0x3c
    li r7, 0x1
    bl fn_800B857C
    b @L_801BDB6C
@L_801BDB24:
    lwz r0, 0x4c(r31)
    rlwinm. r0, r0, 0, 7, 7
    beq @L_801BDB50
    lwz r6, 0x14(r31)
    li r4, 0x1
    lwz r5, 0x10(r31)
    li r7, 0x0
    lwz r3, 0xa4(r31)
    li r8, 0x7d
    bl fn_800B857C
    b @L_801BDB6C
@L_801BDB50:
    lwz r3, 0xa4(r31)
    li r4, 0x1
    lwz r5, 0x10(r31)
    li r6, 0x3c
    lwz r8, 0x14(r31)
    li r7, 0x0
    bl fn_800B857C
@L_801BDB6C:
    bl fn_801A68F0
    li r30, 0x0
    b @L_801BDB8C
@L_801BDB78:
    li r0, 0x1
    slw r0, r0, r30
    and. r0, r3, r0
    bne @L_801BDB94
    addi r30, r30, 0x1
@L_801BDB8C:
    cmpwi r30, 0x8
    blt @L_801BDB78
@L_801BDB94:
    cmpwi r30, 0x8
    blt @L_801BDBA0
    li r30, 0x0
@L_801BDBA0:
    lwz r0, 0xa4(r31)
    cmplwi r0, 0x7
    bgt @L_801BDBFC
    lis r3, jumptable_8036D714@ha
    rlwinm r0, r0, 2, 0, 29
    addi r3, r3, jumptable_8036D714@l
    lwzx r3, r3, r0
    mtctr r3
    bctr
    li r5, 0xc
    b @L_801BDC10
    li r5, 0xd
    b @L_801BDC10
    li r5, 0xe
    b @L_801BDC10
    li r5, 0xf
    b @L_801BDC10
    li r5, 0x10
    b @L_801BDC10
    li r5, 0x11
    b @L_801BDC10
    li r5, 0x12
    b @L_801BDC10
@L_801BDBFC:
    addi r3, r2, -22512
    li r4, 0x756
    addi r5, r2, -22480
    bl fn_80196E10
    li r5, 0xc
@L_801BDC10:
    lis r4, lbl_8036D46C@ha
    lwz r3, 0xa4(r31)
    rlwinm r6, r30, 2, 0, 29
    li r7, 0x0
    addi r0, r4, lbl_8036D46C@l
    addi r3, r3, 0x1
    add r4, r0, r6
    li r6, 0x3c
    lwz r4, 0x0(r4)
    li r8, 0x7d
    bl fn_800B857C
    b @L_801BDD50
@L_801BDC40:
    lwz r0, 0x4c(r31)
    rlwinm r0, r0, 0, 28, 31
    cmplwi r0, 0x4
    bne @L_801BDC70
    lwz r5, 0x10(r31)
    li r4, 0xa
    lwz r3, 0xa4(r31)
    li r6, 0x3c
    li r7, 0x0
    li r8, 0x7d
    bl fn_800B857C
    b @L_801BDD50
@L_801BDC70:
    lwz r0, 0x4c(r31)
    rlwinm r0, r0, 0, 28, 31
    cmpwi r0, 0x4
    beq @L_801BDD08
    bge @L_801BDC98
    cmpwi r0, 0x3
    bge @L_801BDCA8
    cmpwi r0, 0x1
    bge @L_801BDCC8
    b @L_801BDD08
@L_801BDC98:
    cmpwi r0, 0x6
    beq @L_801BDCE8
    bge @L_801BDD08
    b @L_801BDCC8
@L_801BDCA8:
    lwz r3, 0xa4(r31)
    li r4, 0x0
    lwz r8, 0x14(r31)
    li r5, 0x0
    li r6, 0x0
    li r7, 0x0
    bl fn_800B857C
    b @L_801BDD50
@L_801BDCC8:
    lwz r3, 0xa4(r31)
    li r4, 0x0
    lwz r8, 0x14(r31)
    li r5, 0x1
    li r6, 0x1e
    li r7, 0x1
    bl fn_800B857C
    b @L_801BDD50
@L_801BDCE8:
    lwz r3, 0xa4(r31)
    li r4, 0x0
    lwz r8, 0x14(r31)
    li r5, 0x0
    li r6, 0x3c
    li r7, 0x1
    bl fn_800B857C
    b @L_801BDD50
@L_801BDD08:
    lwz r0, 0x4c(r31)
    rlwinm. r0, r0, 0, 7, 7
    beq @L_801BDD34
    lwz r6, 0x14(r31)
    li r4, 0x1
    lwz r5, 0x10(r31)
    li r7, 0x0
    lwz r3, 0xa4(r31)
    li r8, 0x7d
    bl fn_800B857C
    b @L_801BDD50
@L_801BDD34:
    lwz r3, 0xa4(r31)
    li r4, 0x1
    lwz r5, 0x10(r31)
    li r6, 0x3c
    lwz r8, 0x14(r31)
    li r7, 0x0
    bl fn_800B857C
@L_801BDD50:
    lwz r31, 0x8(r31)
@L_801BDD54:
    cmplwi r31, 0x0
    bne @L_801BDA74
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    lwz r30, 0x8(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr

